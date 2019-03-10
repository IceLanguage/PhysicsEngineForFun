using UnityEngine;
using System.Collections;

public class BlobGame : MonoBehaviour
{
    class Platform : PhysicsEngineForFun.IParticleContactGenerator
    {
        public Vector3 PointInPlatform, Normal;
        public PhysicsEngineForFun.Particle CheckParticle;
        public float ParticleRadius;
        public override bool AddContact(PhysicsEngineForFun.ParticleContact contact)
        {
            if (CheckParticle == null)
            {
                Debug.LogError("Please Assign a value to the variable named checkParticle");
                return false;
            }

            Vector3 particlePos = CheckParticle.GetParticlePos();
            Vector3 dir = particlePos - PointInPlatform;
            float dot = Vector3.Dot(dir, Normal);
            float height = Mathf.Sqrt(dir.sqrMagnitude - dot * dot);

            if(dot > 0)
            {
                if (height > ParticleRadius) return false;

                contact.contactNormal = Normal.GetPhysicsEngineVector3();
                contact.particle0 = CheckParticle;
                contact.particle1 = null;
                contact.restitutionCoefficient = 0;
                contact.penetrationDepth = ParticleRadius - height;
                return true;
            }

            contact.contactNormal = Normal.GetPhysicsEngineVector3();
            contact.particle0 = CheckParticle;
            contact.particle1 = null;
            contact.restitutionCoefficient = 0;
            contact.penetrationDepth = height + ParticleRadius;
            return true;
        }
    }

    class BlobForceGenerator : PhysicsEngineForFun.IParticleForceGenerator
    {
        public PhysicsEngineForFun.Particle[] particles;
        public float minNaturalDistance, maxNaturalDistance, maxDistance;
        public float maximumForceToPushApart, maximumForceToPullTogether;
        public float maximumForceToFloat;
        public override void UpdateForce(PhysicsEngineForFun.Particle particle, float duration)
        {
            if(particles == null)
            {
                Debug.LogError("Please Assign a value to the variable named particles");
                return;
            }

            int joinCount = 0;

            if (particles == null)
            {
                Debug.LogError("Please Assign a value to the variable named particles");
                return;
            }

            foreach (var p in particles)
            {
                Vector3 dir = p.GetParticlePos() - particle.GetParticlePos();

                float dis = dir.magnitude;

                if (dis < minNaturalDistance)
                {

                    float a = dis / minNaturalDistance;
                    particle.AddForce(
                        (dir.normalized * a * maximumForceToPushApart * -1.0f)
                        .GetPhysicsEngineVector3()
                    );
                    joinCount++;
                }
                else if (dis > maxNaturalDistance && dis < maxDistance)
                {

                    float a = (dis - maxNaturalDistance) / (maxDistance - maxNaturalDistance);
                    particle.AddForce(
                       (dir.normalized * a * maximumForceToPullTogether)
                       .GetPhysicsEngineVector3()
                        );
                    joinCount++;
                }

                if (p == particle && joinCount > 0)
                {
                    float force = joinCount / particles.Length * maximumForceToFloat;
                    if (force > maximumForceToFloat) force = maximumForceToFloat;
                    particle.AddForce(new PhysicsEngineForFun.Vector3(0, force, 0));
                }
            }
        }

        public override void Dispose()
        {
            base.Dispose();
            for (int i = 0; i < particles.Length; ++i)
            {
                if (particles[i] != null)
                {
                    particles[i].Dispose();
                    particles[i] = null;
                }
            }
        }
    }

    private readonly PhysicsEngineForFun.ParticleWorld world = new PhysicsEngineForFun.ParticleWorld(5, 1);
    private readonly PhysicsEngineForFun.Particle[] particles = new PhysicsEngineForFun.Particle[5];

    public Material Mat;
    public float partcleRadius = 0.4f;

    private BlobForceGenerator blobForceGenerator;
    private readonly Platform[] platforms = new Platform[5];

    private float xmove, ymove;

    private void Start()
    {
        blobForceGenerator = new BlobForceGenerator()
        {
            particles = particles,
            maximumForceToPullTogether = 10f,
            maximumForceToPushApart = 10f,
            maximumForceToFloat = 2f,
            minNaturalDistance = partcleRadius * 0.7f,
            maxNaturalDistance = partcleRadius * 1.4f,
            maxDistance = partcleRadius * 2.1f
        };

        

        for (int i = 0; i < 5; ++i)
        {
            particles[i] = new PhysicsEngineForFun.Particle()
            {
                velocity = new PhysicsEngineForFun.Vector3(0, 0, 0),
                position = new PhysicsEngineForFun.Vector3(Random.Range(2 * i - 5, 2 * i + -3), 3, 0),
                acceleration = new PhysicsEngineForFun.Vector3(0, -2f, 0),
                damping = 0.2f
            };

            particles[i].SetMass(0.2f);
            particles[i].ClearAccumulator();

            world.particles.Add(particles[i]);

            platforms[i] = new Platform()
            {
                PointInPlatform = Vector3.zero,
                Normal = Vector3.up,
                ParticleRadius = partcleRadius,
                CheckParticle = particles[i]
            };

            world.registry.Add(particles[i], blobForceGenerator);

            world.contactGenerators.Add(platforms[i]);
        }
    }

    private void FixedUpdate()
    {
        world.Start();
        xmove *= Mathf.Pow(0.1f, Time.fixedDeltaTime);
        ymove *= Mathf.Pow(0.1f, Time.fixedDeltaTime);
        particles[0].AddForce(new PhysicsEngineForFun.Vector3(xmove, ymove, 0));
        world.RunPhysics(Time.fixedDeltaTime, false);
    }

    private void OnRenderObject()
    {
        for (int i = 0; i < particles.Length; ++i)
        {
            GLDrawSphere(particles[i].GetParticlePos(), partcleRadius,Color.blue, 5);
        }
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.UpArrow))
        {
            ymove += 0.1f ;
        }
        else if (Input.GetKey(KeyCode.DownArrow))
        {
            ymove -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.LeftArrow))
        {
            xmove -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            xmove += 0.1f;
        }
    }

    private void OnDestroy()
    {
        world.Dispose();

        for(int i = 0; i < particles.Length; ++i)
        {
            if(particles[i] != null)
            {
                particles[i].Dispose();
                particles[i] = null;
            }

            if(platforms[i] != null)
            {
                platforms[i].Dispose();
                platforms[i] = null;
            }
        }

        if (blobForceGenerator != null)
        {
            blobForceGenerator.Dispose();
            blobForceGenerator = null;
        }

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
