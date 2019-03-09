using UnityEngine;
using System.Collections;

public class Platform : MonoBehaviour
{
    private readonly PhysicsEngineForFun.ParticleWorld world = new PhysicsEngineForFun.ParticleWorld(6 * 10);
    private readonly PhysicsEngineForFun.Particle[] particles = new PhysicsEngineForFun.Particle[4];
    private readonly PhysicsEngineForFun.ParticleConnectingRod[] connectingRods = new PhysicsEngineForFun.ParticleConnectingRod[15];

    public Vector3 AdditionalMassRealPos;
    public float ParticleMass = 1;
    public float AdditionalMass = 10f;
    private Vector3 AdditionalMassPos = new Vector3(2.5f, 0, 0.5f);

    public Material Mat;

    private void Start()
    {
        for (int i = 0; i < 4; ++i)
        {
            PhysicsEngineForFun.Particle p = new PhysicsEngineForFun.Particle
            {
                velocity = new PhysicsEngineForFun.Vector3(0, 0, 0),
                damping = 0.9f,
                acceleration = new PhysicsEngineForFun.Vector3(0, -9.8f, 0)
            };
            p.ClearAccumulator();

            world.particles.Add(p);

            particles[i] = p;
        }

        particles[0].position = new PhysicsEngineForFun.Vector3(-3, 2, 1);
        particles[1].position = new PhysicsEngineForFun.Vector3(-3, 2, -1);
        particles[2].position = new PhysicsEngineForFun.Vector3(4, 2, 1);
        particles[3].position = new PhysicsEngineForFun.Vector3(4, 2, -1);

        for (int i = 0; i < 15; ++i)
        {
            connectingRods[i] = new PhysicsEngineForFun.ParticleConnectingRod();
            world.contactGenerators.Add(connectingRods[i]);
        }

        connectingRods[0].particle0 = particles[0];
        connectingRods[0].particle1 = particles[1];
        connectingRods[0].Length = 2;
        connectingRods[1].particle0 = particles[2];
        connectingRods[1].particle1 = particles[3];
        connectingRods[1].Length = 2;
        connectingRods[2].particle0 = particles[4];
        connectingRods[2].particle1 = particles[5];
        connectingRods[2].Length = 2;

        connectingRods[3].particle0 = particles[2];
        connectingRods[3].particle1 = particles[4];
        connectingRods[3].Length = 7;
        connectingRods[4].particle0 = particles[3];
        connectingRods[4].particle1 = particles[5];
        connectingRods[4].Length = 7;

        connectingRods[5].particle0 = particles[0];
        connectingRods[5].particle1 = particles[2];
        connectingRods[5].Length = 3.606f;
        connectingRods[6].particle0 = particles[1];
        connectingRods[6].particle1 = particles[3];
        connectingRods[6].Length = 3.606f;

        connectingRods[7].particle0 = particles[0];
        connectingRods[7].particle1 = particles[4];
        connectingRods[7].Length = 4.472f;
        connectingRods[8].particle0 = particles[1];
        connectingRods[8].particle1 = particles[5];
        connectingRods[8].Length = 4.472f;

        connectingRods[9].particle0 = particles[0];
        connectingRods[9].particle1 = particles[3];
        connectingRods[9].Length = 4.123f;
        connectingRods[10].particle0 = particles[2];
        connectingRods[10].particle1 = particles[5];
        connectingRods[10].Length = 7.28f;
        connectingRods[11].particle0 = particles[4];
        connectingRods[11].particle1 = particles[1];
        connectingRods[11].Length = 4.899f;
        connectingRods[12].particle0 = particles[1];
        connectingRods[12].particle1 = particles[2];
        connectingRods[12].Length = 4.123f;
        connectingRods[13].particle0 = particles[3];
        connectingRods[13].particle1 = particles[4];
        connectingRods[13].Length = 7.28f;
        connectingRods[14].particle0 = particles[5];
        connectingRods[14].particle1 = particles[0];
        connectingRods[14].Length = 4.899f;

        UpdateAdditionalMass();
    }

    private void OnDestroy()
    {
        for(int i = 0; i < 6; ++i)
        {
            if(particles[i] != null)
            {
                particles[i].Dispose();
                particles[i] = null;
            }
        }

        for (int i = 0; i < 15; ++i)
        {
            if (connectingRods[i] != null)
            {
                connectingRods[i].Dispose();
                connectingRods[i] = null;
            }
        }

        world.Dispose();
    }

    private void Update()
    {
        Vector3 v = AdditionalMassPos;

        if (Input.GetKey(KeyCode.UpArrow))
        {
            v.z += 0.1f;
        }
        else if (Input.GetKey(KeyCode.DownArrow))
        {
            v.z -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.LeftArrow))
        {
            v.x -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            v.x += 0.1f;
        }

        AdditionalMassPos = v;
    }

    private void FixedUpdate()
    {
        world.RunPhysics(Time.fixedDeltaTime);

        UpdateAdditionalMass();
    }

    private void UpdateAdditionalMass()
    {
        for (int i = 0; i < 6; ++i) particles[i].SetMass(ParticleMass);
        AdditionalMassPos.x = Mathf.Clamp(AdditionalMassPos.x, 0, 1);
        AdditionalMassPos.z = Mathf.Clamp(AdditionalMassPos.z, 0, 1);
        int x = 0;
        int z = 0;
        float xp = AdditionalMassPos.x - x;
        float zp = AdditionalMassPos.z - z;

        int a1 = 1;
        int a2 = 0;
        int a3 = 3;
        int a4 = 2;

        float t1 = (1 - xp) * (1 - zp);
        float t3 = xp * (1 - zp);
        float t2 = (1 - xp) * zp;
        float t4 = xp * zp;

        particles[a1].SetMass(ParticleMass + AdditionalMass * t1);
        particles[a2].SetMass(ParticleMass + AdditionalMass * t2);
        particles[a3].SetMass(ParticleMass + AdditionalMass * t3);
        particles[a4].SetMass(ParticleMass + AdditionalMass * t4);

        Vector3 p1 = new Vector3(particles[a1].position.x, particles[a1].position.y, particles[a1].position.z);
        Vector3 p2 = new Vector3(particles[a2].position.x, particles[a2].position.y, particles[a2].position.z);
        Vector3 p3 = new Vector3(particles[a3].position.x, particles[a3].position.y, particles[a3].position.z);
        Vector3 p4 = new Vector3(particles[a4].position.x, particles[a4].position.y, particles[a4].position.z);

        AdditionalMassRealPos = p1 * t1 + p2 * t2 + p3 * t3 + p4 * t4;
    }

    private void OnRenderObject()
    {
        GL.PushMatrix();
        Mat.SetPass(0);

        
        GL.Begin(GL.LINES);
        GL.Color(Color.green);
        for (int i = 0; i < 15; ++i)
        {
            if (connectingRods[i] == null) break;
            GL.Vertex3(connectingRods[i].particle0.position.x, connectingRods[i].particle0.position.y, connectingRods[i].particle0.position.z);
            GL.Vertex3(connectingRods[i].particle1.position.x, connectingRods[i].particle1.position.y, connectingRods[i].particle1.position.z);
        }
        GL.End();

        for (int i = 0; i < 6; ++i)
        {
            GLDrawSphere(new Vector3(particles[i].position.x, particles[i].position.y, particles[i].position.z),
                0.1f,
                Color.yellow);
        }

        GLDrawSphere(AdditionalMassRealPos,
                0.3f,
                Color.red);

        GL.PopMatrix();
    }

    private void GLDrawSphere(Vector3 center, float radius, Color color, int n = 3)
    {
        GL.Begin(GL.TRIANGLES);
        GL.Color(color);

        Vector3 a = new Vector3(0.0f, 0.0f, -1.0f);
        Vector3 b = new Vector3(0.0f, 0.942809f, 0.333333f);
        Vector3 c = new Vector3(-0.816497f, -0.471405f, 0.333333f);
        Vector3 d = new Vector3(0.816497f, -0.471405f, 0.333333f);

        DivideTriangle(a, b, c, n, center, radius);
        DivideTriangle(d, c, b, n, center, radius);
        DivideTriangle(a, d, b, n, center, radius);
        DivideTriangle(a, c, d, n, center, radius);

        GL.End();
    }

    private void DivideTriangle(Vector3 a, Vector3 b, Vector3 c, int count, Vector3 center, float radius)
    {
        if (count > 0)
        {
            Vector3 ab = a * 0.5f + b * 0.5f; ab.Normalize();
            Vector3 ac = a * 0.5f + c * 0.5f; ac.Normalize();
            Vector3 bc = b * 0.5f + c * 0.5f; bc.Normalize();

            DivideTriangle(a, ab, ac, count - 1, center, radius);
            DivideTriangle(ab, b, bc, count - 1, center, radius);
            DivideTriangle(bc, c, ac, count - 1, center, radius);
            DivideTriangle(ab, bc, ac, count - 1, center, radius);
        }
        else
        {
            GL.Vertex(a * radius + center);
            GL.Vertex(b * radius + center);
            GL.Vertex(c * radius + center);
        }
    }
}
