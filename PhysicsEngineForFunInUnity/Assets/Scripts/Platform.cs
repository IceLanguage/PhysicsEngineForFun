using UnityEngine;
using System.Collections;

public class Platform : MonoBehaviour
{
    private readonly PhysicsEngineForFun.ParticleWorld world = new PhysicsEngineForFun.ParticleWorld(6 * 10);
    private readonly PhysicsEngineForFun.Particle[] particles = new PhysicsEngineForFun.Particle[4];
    private readonly PhysicsEngineForFun.ParticleConnectingRod[] connectingRods = new PhysicsEngineForFun.ParticleConnectingRod[6];
    private readonly PhysicsEngineForFun.ParticleConnectingRodConstraint[] rodsConstraints = new PhysicsEngineForFun.ParticleConnectingRodConstraint[8];

    private readonly PhysicsEngineForFun.Vector3[] fixedPoints = new PhysicsEngineForFun.Vector3[2]
    {
        new PhysicsEngineForFun.Vector3(0, 0 , 1),
        new PhysicsEngineForFun.Vector3(0, 0 ,-1)
    };

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

        for (int i = 0; i < 6; ++i)
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
        connectingRods[2].particle0 = particles[0];
        connectingRods[2].particle1 = particles[2];
        connectingRods[2].Length = 7;
        connectingRods[3].particle0 = particles[1];
        connectingRods[3].particle1 = particles[3];
        connectingRods[3].Length = 7;
        connectingRods[4].particle0 = particles[0];
        connectingRods[4].particle1 = particles[3];
        connectingRods[4].Length = 7.28f;     
        connectingRods[5].particle0 = particles[1];
        connectingRods[5].particle1 = particles[2];
        connectingRods[5].Length = 7.28f;

        for (int i = 0; i < 8; ++i)
        {
            rodsConstraints[i] = new PhysicsEngineForFun.ParticleConnectingRodConstraint();
            world.contactGenerators.Add(rodsConstraints[i]);
        }

        rodsConstraints[0].anchorPoint = fixedPoints[0];
        rodsConstraints[0].particle = particles[0];
        rodsConstraints[0].Length = 3.606f;
        rodsConstraints[1].anchorPoint = fixedPoints[1];
        rodsConstraints[1].particle = particles[1];
        rodsConstraints[1].Length = 3.606f;
        rodsConstraints[2].anchorPoint = fixedPoints[0];
        rodsConstraints[2].particle = particles[2];
        rodsConstraints[2].Length = 4.472f;
        rodsConstraints[3].anchorPoint = fixedPoints[1];
        rodsConstraints[3].particle = particles[3];
        rodsConstraints[3].Length = 4.472f;
        rodsConstraints[4].anchorPoint = fixedPoints[0];
        rodsConstraints[4].particle = particles[1];
        rodsConstraints[4].Length = 4.123f;
        rodsConstraints[5].particle = particles[3];
        rodsConstraints[5].anchorPoint = fixedPoints[0];
        rodsConstraints[5].Length = 4.899f;
        rodsConstraints[6].particle = particles[2];
        rodsConstraints[6].anchorPoint = fixedPoints[1];
        rodsConstraints[6].Length = 4.899f;
        rodsConstraints[7].anchorPoint = fixedPoints[1];
        rodsConstraints[7].particle = particles[0];
        rodsConstraints[7].Length = 4.123f;

        UpdateAdditionalMass();
    }

    private void OnDestroy()
    {
        for(int i = 0; i < 4; ++i)
        {
            if(particles[i] != null)
            {
                particles[i].Dispose();
                particles[i] = null;
            }
        }

        for (int i = 0; i < 6; ++i)
        {
            if (connectingRods[i] != null)
            {
                connectingRods[i].Dispose();
                connectingRods[i] = null;
            }
        }

        for (int i = 0; i < 8; ++i)
        {
            if (rodsConstraints[i] != null)
            {
                rodsConstraints[i].Dispose();
                rodsConstraints[i] = null;
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
        for (int i = 0; i < 4; ++i) particles[i].SetMass(ParticleMass);
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

        Vector3 p1 = particles[a1].GetParticlePos();
        Vector3 p2 = particles[a2].GetParticlePos();
        Vector3 p3 = particles[a3].GetParticlePos();
        Vector3 p4 = particles[a4].GetParticlePos();

        AdditionalMassRealPos = p1 * t1 + p2 * t2 + p3 * t3 + p4 * t4;
    }

    private void OnRenderObject()
    {
        GL.PushMatrix();
        Mat.SetPass(0);

        GL.Begin(GL.LINES);
        GL.Color(Color.green);
        for (int i = 0; i < 6; ++i)
        {
            if (connectingRods[i] == null) break;
            GL.Vertex3(connectingRods[i].particle0.position.x, connectingRods[i].particle0.position.y, connectingRods[i].particle0.position.z);
            GL.Vertex3(connectingRods[i].particle1.position.x, connectingRods[i].particle1.position.y, connectingRods[i].particle1.position.z);
        }
        GL.Color(Color.cyan);
        for (int i = 0; i < 8; ++i)
        {
            if (rodsConstraints[i] == null) break;
            GL.Vertex3(rodsConstraints[i].particle.position.x, rodsConstraints[i].particle.position.y, rodsConstraints[i].particle.position.z);
            GL.Vertex3(rodsConstraints[i].anchorPoint.x, rodsConstraints[i].anchorPoint.y, rodsConstraints[i].anchorPoint.z);
        }
        GL.End();

        for (int i = 0; i < 4; ++i)
        {
            GLDrawSphere(new Vector3(particles[i].position.x, particles[i].position.y, particles[i].position.z),
                0.1f,
                Color.yellow, 3);
        }
        GL.PopMatrix();
        GLDrawSphere(AdditionalMassRealPos,
                0.3f,
                Color.red, 3);

        GLDrawSphere(new Vector3(fixedPoints[0].x, fixedPoints[0].y, fixedPoints[0].z),
                0.1f,
                Color.black, 3);

        GLDrawSphere(new Vector3(fixedPoints[1].x, fixedPoints[1].y, fixedPoints[1].z),
                0.1f,
                Color.black, 3);

        
    }

    private void GLDrawSphere(Vector3 center, float radius, Color color, int n = 3)
    {
        GL.PushMatrix();
        Mat.SetPass(0);
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
        GL.PopMatrix();
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
