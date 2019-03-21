using UnityEngine;
using System.Collections;

public class Sailboat : MonoBehaviour
{
    private PhysicsEngineForFun.BuoyancyOnRigidBody buoyancy;
    private readonly PhysicsEngineForFun.RigidBody sailboat = new PhysicsEngineForFun.RigidBody();
    private PhysicsEngineForFun.AeroForceOnRigidBody sail;
    private readonly PhysicsEngineForFun.Vector3 PhysicsEngineWindSpeed = new PhysicsEngineForFun.Vector3(0, 0, 0);
    private readonly PhysicsEngineForFun.RigidBodyForceRegistry registry = new PhysicsEngineForFun.RigidBodyForceRegistry();

    public Vector3 windSpeed;
    public Vector3 showSailboatVelocity;

    private void Start()
    {
        sail = new PhysicsEngineForFun.AeroForceOnRigidBody(
            new PhysicsEngineForFun.Matrix3(0, 0, 0, 0, 0, 0, 0, 0, -1.0f),
            new PhysicsEngineForFun.Vector3(2.0f, 0, 0),
            PhysicsEngineWindSpeed);

        buoyancy = new PhysicsEngineForFun.BuoyancyOnRigidBody(
            new PhysicsEngineForFun.Vector3(0.0f, 0.5f, 0.0f),
            1.0f,3.0f,1.6f);

        sailboat.position = new PhysicsEngineForFun.Vector3(0, 1.6f, 0);
        sailboat.orientation = new PhysicsEngineForFun.Quaternion(1, 0, 0, 0);
        sailboat.velocity = new PhysicsEngineForFun.Vector3(0, 0, 0);
        sailboat.rotation = new PhysicsEngineForFun.Vector3(0, 0, 0);
        sailboat.SetMass(200f);

        PhysicsEngineForFun.Matrix3 i = new PhysicsEngineForFun.Matrix3();
        i.SetBlockInertiaTensor(new PhysicsEngineForFun.Vector3(2, 2, 2), 100f);
        sailboat.SetInertiaTensor(i);

        sailboat.linearDamping = sailboat.angularDamping = 0.8f;
        sailboat.acceleration = new PhysicsEngineForFun.Vector3(0, -9.8f, 0);
        sailboat.CalculateDerivedData();

        registry.Add(sailboat, sail);
        registry.Add(sailboat, buoyancy);

        Vector3 pos = transform.position = sailboat.position.GetUnityEngineVector3();
        transform.localEulerAngles = sailboat.rotation.GetUnityEngineVector3();

        Vector3 forward = pos - Camera.main.transform.position;
        forward.Normalize();
        if (forward == Vector3.zero) return;
        Camera.main.transform.rotation = Quaternion.LookRotation(forward);
    }

    private void FixedUpdate()
    {
        windSpeed = windSpeed * 0.9f + new Vector3(Random.Range(0, 0.1f), 0, Random.Range(0, 0.1f));

        PhysicsEngineWindSpeed.x = windSpeed.x;
        PhysicsEngineWindSpeed.y = windSpeed.y;
        PhysicsEngineWindSpeed.z = windSpeed.z;

        sailboat.ClearAccumulator();
        PhysicsEngineForFun.Vector3 propulsion = new PhysicsEngineForFun.Vector3(-10.0f, 0, 0);
        propulsion = sailboat.transformMatrix.TransformDirection(propulsion);

        sailboat.AddForce(propulsion);

        registry.UpdateForces(0.02f);

        sailboat.Integrate(0.02f);

        showSailboatVelocity = sailboat.velocity.GetUnityEngineVector3();
    }

    private void LateUpdate()
    {
        transform.position = sailboat.position.GetUnityEngineVector3();
        transform.localEulerAngles = sailboat.rotation.GetUnityEngineVector3();

        //Vector3 forward = pos - Camera.main.transform.position;
        //forward.Normalize();
        //if (forward == Vector3.zero) return;
        //Camera.main.transform.rotation = Quaternion.LookRotation(forward);
    }

    private void OnDestroy()
    {
        if(buoyancy != null)
        {
            buoyancy.Dispose();
            buoyancy = null;
        }

        if (sailboat != null) sailboat.Dispose();

        if(sail != null)
        {
            sail.Dispose();
            sail = null;
        }

        if (PhysicsEngineWindSpeed != null)
        {
            PhysicsEngineWindSpeed.Dispose();
        }

        if (registry != null)
        {
            registry.Dispose();
        }
    }
}
