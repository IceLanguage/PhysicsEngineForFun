using UnityEngine;
using System.Collections;

public class Bridge : MonoBehaviour
{
    private readonly PhysicsEngineForFun.ParticleWorld world = new PhysicsEngineForFun.ParticleWorld(12 * 10);
    private readonly PhysicsEngineForFun.Particle[] particles = new PhysicsEngineForFun.Particle[12];
    private readonly PhysicsEngineForFun.ParticleCable[] cables = new PhysicsEngineForFun.ParticleCable[10];
    private readonly PhysicsEngineForFun.ParticleCableConstraint[] constraints = new PhysicsEngineForFun.ParticleCableConstraint[12];
    private readonly PhysicsEngineForFun.ParticleConnectingRod[] connectingRods = new PhysicsEngineForFun.ParticleConnectingRod[6];

    public Vector3 AdditionalMassRealPos;
    public float ParticleMass = 1;
    public float AdditionalMass = 10f;
    private Vector3 AdditionalMassPos = new Vector3(2.5f, 0, 0.5f);

    public Material Mat;

    private void Start()
    {
        for (int i = 0; i < 12; ++i)
        {
            PhysicsEngineForFun.Particle p = new PhysicsEngineForFun.Particle
            {
                position = new PhysicsEngineForFun.Vector3(i / 2, 0, i % 2),
                velocity = new PhysicsEngineForFun.Vector3(0, 0, 0),
                damping = 0.9f,
                acceleration = new PhysicsEngineForFun.Vector3(0, -9.8f, 0)
            };
            p.ClearAccumulator();

            world.particles.Add(p);

            particles[i] = p;
        }

        for (int i = 0; i < 10; ++i)
        {
            PhysicsEngineForFun.ParticleCable c = new PhysicsEngineForFun.ParticleCable
            {
                particle0 = particles[i],
                particle1 = particles[i + 2],
                maxLength = 2f,
                restitutionCoefficient = 0.3f
            };

            world.contactGenerators.Add(c);

            cables[i] = c;
        }

        for (int i = 0; i < 12; ++i)
        {
            PhysicsEngineForFun.ParticleCableConstraint c = new PhysicsEngineForFun.ParticleCableConstraint()
            {
                particle = particles[i],
                anchorPoint = new PhysicsEngineForFun.Vector3(i / 2 * 2f + 4f, 4, i % 2 * 1.4f - 3.2f),
                maxLength = i < 6 ? i / 2 * 0.5f + 3f : (5 - i / 2) * 0.5f + 3f,
                restitutionCoefficient = 0.5f
            };

            world.contactGenerators.Add(c);

            constraints[i] = c;
        }

        for (int i = 0; i < 6; ++i)
        {
            PhysicsEngineForFun.ParticleConnectingRod r = new PhysicsEngineForFun.ParticleConnectingRod()
            {
                particle0 = particles[i * 2],
                particle1 = particles[i * 2 + 1],
                Length = 2f
            };

            world.contactGenerators.Add(r);

            connectingRods[i] = r;
        }

        UpdateAdditionalMass();
    }

    private void OnDestroy()
    {
        for (int i = 0; i < 12; ++i)
        {
            if (particles[i] != null)
            {
                particles[i].Dispose();
                particles[i] = null;
            }

            if (constraints[i] != null)
            {
                constraints[i].Dispose();
                constraints[i] = null;
            }
        }

        for (int i = 0; i < 10; ++i)
        {
            if (cables[i] != null)
            {
                cables[i].Dispose();
                cables[i] = null;
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
        for (int i = 0; i < 12; ++i) particles[i].SetMass(ParticleMass);
        AdditionalMassPos.x = Mathf.Clamp(AdditionalMassPos.x, 0, 5);
        AdditionalMassPos.z = Mathf.Clamp(AdditionalMassPos.z, 0, 1);
        int x = (int)AdditionalMassPos.x;
        if (x == 5) x = 4;
        int z = 0;
        float xp = AdditionalMassPos.x - x;
        float zp = AdditionalMassPos.z - z;

        int a1 = x * 2 + z;
        int a2 = a1 + 1;
        int a3 = a1 + 2;
        int a4 = a1 + 3;

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

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.blue;
        for (int i = 0; i < 12; ++i)
        {
            if (particles[i] == null) break;
            Gizmos.DrawSphere(new Vector3(particles[i].position.x, particles[i].position.y, particles[i].position.z),
                0.1f);
        }

        Gizmos.color = Color.red;
        Gizmos.DrawSphere(AdditionalMassRealPos, 0.3f);

        Gizmos.color = Color.yellow;
        for (int i = 0; i < 12; ++i)
        {
            if (constraints[i] == null) break;
            Gizmos.DrawLine(
                new Vector3(constraints[i].particle.position.x, constraints[i].particle.position.y, constraints[i].particle.position.z),
                new Vector3(constraints[i].anchorPoint.x, constraints[i].anchorPoint.y, constraints[i].anchorPoint.z));
        }

        Gizmos.color = Color.green;
        for (int i = 0; i < 10; ++i)
        {
            if (cables[i] == null) break;
            Gizmos.DrawLine(
                new Vector3(cables[i].particle0.position.x, cables[i].particle0.position.y, cables[i].particle0.position.z),
                new Vector3(cables[i].particle1.position.x, cables[i].particle1.position.y, cables[i].particle1.position.z));

        }

        Gizmos.color = Color.cyan;
        for (int i = 0; i < 6; ++i)
        {
            if (connectingRods[i] == null) break;
            Gizmos.DrawLine(
               new Vector3(connectingRods[i].particle0.position.x, connectingRods[i].particle0.position.y, connectingRods[i].particle0.position.z),
               new Vector3(connectingRods[i].particle1.position.x, connectingRods[i].particle1.position.y, connectingRods[i].particle1.position.z));

        }
    }

    private void OnRenderObject()
    {
        GL.PushMatrix();
        Mat.SetPass(0);

        GL.Color(Color.cyan);
        GL.Begin(GL.LINES);
        for (int i = 0; i < 6; ++i)
        {
            if (connectingRods[i] == null) break;
            GL.Vertex3(connectingRods[i].particle0.position.x, connectingRods[i].particle0.position.y, connectingRods[i].particle0.position.z);
            GL.Vertex3(connectingRods[i].particle1.position.x, connectingRods[i].particle1.position.y, connectingRods[i].particle1.position.z);
        }
        GL.Color(Color.green);
        for (int i = 0; i < 10; ++i)
        {
            if (cables[i] == null) break;
            GL.Vertex3(cables[i].particle0.position.x, cables[i].particle0.position.y, cables[i].particle0.position.z);
            GL.Vertex3(cables[i].particle1.position.x, cables[i].particle1.position.y, cables[i].particle1.position.z);
        }
        GL.Color(Color.yellow);
        for (int i = 0; i < 12; ++i)
        {
            if (constraints[i] == null) break;
            GL.Vertex3(constraints[i].particle.position.x, constraints[i].particle.position.y, constraints[i].particle.position.z);
            GL.Vertex3(constraints[i].anchorPoint.x, constraints[i].anchorPoint.y, constraints[i].anchorPoint.z);
        }
        GL.End();

        for (int i = 0; i < 12; ++i)
        {
            GLDrawSphere(new Vector3(particles[i].position.x, particles[i].position.y, particles[i].position.z),
                0.1f,
                Color.blue);
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

    private void DivideTriangle(Vector3 a, Vector3 b, Vector3 c, int count,Vector3 center, float radius)
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
