
static class PhysicsEngineForFunHelp
{
    public static UnityEngine.Vector3 GetParticlePos(this PhysicsEngineForFun.Particle particle)
    {
        return new UnityEngine.Vector3(particle.position.x, particle.position.y, particle.position.z);
    }

    public static PhysicsEngineForFun.Vector3 GetPhysicsEngineVector3(this UnityEngine.Vector3 v)
    {
        return new PhysicsEngineForFun.Vector3(v.x, v.y, v.z);
    }

    public static UnityEngine.Vector3 GetUnityEngineVector3(this PhysicsEngineForFun.Vector3 v)
    {
        return new UnityEngine.Vector3(v.x, v.y, v.z);
    }

    //public static UnityEngine.Quaternion GetUnityEngineQuaternion(this PhysicsEngineForFun.Quaternion q)
    //{
    //    return new UnityEngine.Quaternion(q.Get_r(),  q.Get_i(), q.Get_j(),  q.Get_k());
    //}
}
