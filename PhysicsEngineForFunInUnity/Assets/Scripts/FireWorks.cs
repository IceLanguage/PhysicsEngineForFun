using System.Collections.Generic;
using UnityEngine;

public class FireWorks : MonoBehaviour
{
    private class FireParticle : System.IDisposable
    {
        public PhysicsEngineForFun.Particle particle = new PhysicsEngineForFun.Particle();
        public float age;
        public int treeDepth;
        public Color color;

        public FireParticle() 
        {
            age = Random.Range(0.5f, 1.4f);
            treeDepth = Random.Range(3, 10);
            particle.damping = Random.Range(0.01f, 1.0f);
            color = Random.ColorHSV();
            color.a = Mathf.Clamp(color.a, 0.01f, 1f);
            color.g = Mathf.Clamp(color.g, 0.01f, 1f);
            color.b = Mathf.Clamp(color.b, 0.01f, 1f);
            particle.SetMass(1f);
            particle.velocity = new PhysicsEngineForFun.Vector3(Random.Range(-5f, 5f), Random.Range(-5f, 5f), Random.Range(-5f, 5f));
        }

        public bool Update(float duration)
        {
            particle.Integrate(duration);
            age -= duration;

            return age > 0f;
        }

        #region IDisposable Support
        private bool disposedValue = false; // 要检测冗余调用

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: 释放托管状态(托管对象)。
                }

                // TODO: 释放未托管的资源(未托管的对象)并在以下内容中替代终结器。
                // TODO: 将大型字段设置为 null。
                particle.Dispose();
                particle = null;

                disposedValue = true;
            }
        }

        // TODO: 仅当以上 Dispose(bool disposing) 拥有用于释放未托管资源的代码时才替代终结器。
        ~FireParticle()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(false);
        }

        // 添加此代码以正确实现可处置模式。
        public void Dispose()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(true);
            // TODO: 如果在以上内容中替代了终结器，则取消注释以下行。
            System.GC.SuppressFinalize(this);
        }
        #endregion
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
                    //childParticle.age = Random.Range(0, particle.age);
                    childParticle.particle.position = particle.particle.position;
                    childParticle.particle.velocity.x += particle.particle.velocity.x;
                    childParticle.particle.velocity.y += particle.particle.velocity.y;
                    childParticle.particle.velocity.z += particle.particle.velocity.z;
                    childParticle.treeDepth = Random.Range(0, particle.treeDepth);
                    childParticle.color = particle.color + new Color(Random.Range(-0.10f, 0.10f), Random.Range(-0.10f, 0.10f), Random.Range(-0.10f, 0.10f));
                    fireParticles.Enqueue(childParticle);
                }
                particle.Dispose();
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
