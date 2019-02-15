using System.Collections.Generic;
using UnityEngine;

public class FireWorks : MonoBehaviour
{
    private class FireParticle
    {
        public PhysicsEngineForFun.Particle particle = new PhysicsEngineForFun.Particle();
        public float age;
        public int treeDepth;
        public Color color;

        public FireParticle()
        {
            age = Random.Range(0.05f, 0.4f);
            treeDepth = Random.Range(1, 10);
            particle.damping = Random.Range(0.01f, 1.0f);
            color = Random.ColorHSV();
            particle.SetMass(1f);
            particle.velocity = new PhysicsEngineForFun.Vector3(Random.Range(-5f, 5f), Random.Range(-5f, 5f), Random.Range(-5f, 5f));
        }

        public bool Update(float duration)
        {
            particle.Integrate(duration);
            age -= duration;

            return age > 0f;
        }
    }

    private ParticleSystem mParticleSystem;

    private Queue<FireParticle> fireParticles = new Queue<FireParticle>();

    private void Awake()
    {
        mParticleSystem = GetComponent<ParticleSystem>();
    }

    private void Update()
    {
        if(Input.GetKeyDown(KeyCode.Space))
        {
            Fire();
        }

        mParticleSystem.Clear();
        ParticleSystem.EmitParams ep = new ParticleSystem.EmitParams();
        foreach(FireParticle particle in fireParticles)
        {
            ep.position = new Vector3(particle.particle.position.x, particle.particle.position.y, particle.particle.position.z);
            ep.startSize = 0.3f;
            ep.startColor = particle.color;
            Debug.Log(ep.startColor);
            mParticleSystem.Emit(ep, 1);
        }
    }

    private void FixedUpdate()
    {
        int count = fireParticles.Count;

        for(int i = 0; i < count; ++i)
        {
            FireParticle particle = fireParticles.Dequeue();

            if(!particle.Update(Time.fixedDeltaTime))
            {
                for(int j = 0; j < particle.treeDepth; ++j)
                {
                    FireParticle childParticle = new FireParticle();
                    childParticle.particle.position = particle.particle.position;
                    childParticle.particle.velocity.x += particle.particle.velocity.x;
                    childParticle.particle.velocity.y += particle.particle.velocity.y;
                    childParticle.particle.velocity.z += particle.particle.velocity.z;
                    childParticle.treeDepth = Random.Range(0, particle.treeDepth);
                    fireParticles.Enqueue(childParticle);
                }
                
            }
            else
            {
                fireParticles.Enqueue(particle);
            }
        }
    }

    private void Fire()
    {
        FireParticle particle = new FireParticle();
        fireParticles.Enqueue(particle);
    }
}
