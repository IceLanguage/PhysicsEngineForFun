using UnityEngine;
using System.Collections;

public class BlobGame : MonoBehaviour
{
    class Platform :PhysicsEngineForFun.IParticleContactGenerator
    {
        public Vector3 pointInPlatform, normal;
        public PhysicsEngineForFun.Particle checkParticle;

        public override bool AddContact(PhysicsEngineForFun.ParticleContact contact)
        {
            return base.AddContact(contact);
        }
    }
}
