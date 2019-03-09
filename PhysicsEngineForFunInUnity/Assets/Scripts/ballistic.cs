using System.Collections.Generic;
using UnityEngine;

public class ballistic : MonoBehaviour {

    public enum ShootType
    {
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    }

    private class Ammo : System.IDisposable
    {
        public PhysicsEngineForFun.Particle particle;
        public uint startTime;
        public GameObject go;

        public Ammo(GameObject go)
        {
            particle = new PhysicsEngineForFun.Particle();
            startTime = 0;
            this.go = GameObject.Instantiate(go);    
        }
        
        public void SerPosition(UnityEngine.Vector3 v)
        {
            particle.position = new PhysicsEngineForFun.Vector3(v.x, v.y, v.z);
            go.transform.position = v;
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
                if(particle != null)
                {
                    particle.Dispose();
                    particle = null;
                }
                go = null;

                disposedValue = true;
            }
        }

        // TODO: 仅当以上 Dispose(bool disposing) 拥有用于释放未托管资源的代码时才替代终结器。
        // ~Ammo() {
        //   // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
        //   Dispose(false);
        // }

        // 添加此代码以正确实现可处置模式。
        public void Dispose()
        {
            // 请勿更改此代码。将清理代码放入以上 Dispose(bool disposing) 中。
            Dispose(true);
            // TODO: 如果在以上内容中替代了终结器，则取消注释以下行。
            // GC.SuppressFinalize(this);
        }
        #endregion
    }

    public ShootType shootType;
    public GameObject BallPrefab;

    private List<Ammo> ammos = new List<Ammo>();

    private void Update()
    {
        if(Input.GetKeyDown(KeyCode.A))
        {
            Fire();
        }
    }

    private void FixedUpdate()
    {
        int len = ammos.Count;
        for(int i = len - 1; i >= 0; i--)
        {
            Ammo a = ammos[i];

            if(Mathf.Abs(a.particle.position.x) > 60f || a.particle.position.y < 0f)
            {
                a.particle.Dispose();
                Destroy(a.go);
                a = null;
                ammos.RemoveAt(i);
                continue;
            }

            a.particle.Integrate(Time.fixedDeltaTime);

            a.go.transform.position = new UnityEngine.Vector3(a.particle.position.x, a.particle.position.y, a.particle.position.z);
        }
    }

    private void Fire()
    {
        Ammo ammo = new Ammo(BallPrefab);
        switch(shootType)
        {
            case ShootType.PISTOL:
                ammo.particle.SetMass(2.0f);
                ammo.particle.velocity = new PhysicsEngineForFun.Vector3(-35.0f, 0.0f, 0.0f);
                ammo.particle.acceleration = new PhysicsEngineForFun.Vector3(0.0f, -1.0f, 0.0f);
                ammo.particle.damping = 0.99f;
                break;
            case ShootType.ARTILLERY:
                ammo.particle.SetMass(200.0f);
                ammo.particle.velocity = new PhysicsEngineForFun.Vector3(-10.0f, 7.0f, 0.0f);
                ammo.particle.acceleration = new PhysicsEngineForFun.Vector3(0.0f, -4.5f, 0.0f);
                ammo.particle.damping = 0.99f;
                break;
            case ShootType.LASER:
                ammo.particle.SetMass(1.0f);
                ammo.particle.velocity = new PhysicsEngineForFun.Vector3(-100.0f, 0.0f, 0.0f);
                ammo.particle.acceleration = new PhysicsEngineForFun.Vector3(0.0f, -0.0f, 0.0f);
                ammo.particle.damping = 0.99f;
                break;
            case ShootType.FIREBALL:
                ammo.particle.SetMass(1.0f);
                ammo.particle.velocity = new PhysicsEngineForFun.Vector3(-10.0f, 0.0f, 0.0f);
                ammo.particle.acceleration = new PhysicsEngineForFun.Vector3(0.0f, -0.5f, 0.0f);
                ammo.particle.damping = 0.99f;
                break;
            default:
                break;
        }
        ammo.SerPosition(BallPrefab.transform.position);
        ammos.Add(ammo);
    }

    private void OnDestroy()
    {
        int size = ammos.Count;
        for(int i = 0;i < size; ++i)
        {
            if (ammos[i] != null)
            {
                ammos[i].Dispose();
                ammos[i] = null;
            }   
        }
    }
}
