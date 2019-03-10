
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
}
