using UnityEngine;
using System.Collections;

public class FlightSim : MonoBehaviour
{
    private PhysicsEngineForFun.AeroForceControlOnRigidBody left_wing, right_wing, rudder;
    private PhysicsEngineForFun.AeroForceOnRigidBody tail;
    private readonly PhysicsEngineForFun.RigidBody aircraft = new PhysicsEngineForFun.RigidBody();
    private readonly PhysicsEngineForFun.RigidBodyForceRegistry registry = new PhysicsEngineForFun.RigidBodyForceRegistry();
    private readonly PhysicsEngineForFun.Vector3 PhysicsEngineWindSpeed = new PhysicsEngineForFun.Vector3(0, 0, 0);
    private float left_wing_control;
    private float right_wing_control;
    private float rudder_control;
    public Vector3 windSpeed;

    private void Start()
    {

        tail = new PhysicsEngineForFun.AeroForceOnRigidBody(
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -1, -0.5f, 0, 0, 0, -0.1f),
            new PhysicsEngineForFun.Vector3(2.0f, 0, 0),
            PhysicsEngineWindSpeed);

        left_wing = new PhysicsEngineForFun.AeroForceControlOnRigidBody(
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -1, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -0.995f, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -1.005f, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Vector3(-1, 0, -2),
            PhysicsEngineWindSpeed);
        left_wing.controlSetting = 0;

       right_wing = new PhysicsEngineForFun.AeroForceControlOnRigidBody(
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -1, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -0.995f, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, -1.005f, -0.5f, 0, 0, 0, 0),
            new PhysicsEngineForFun.Vector3(-1, 0, 2),
            PhysicsEngineWindSpeed);
        right_wing.controlSetting = 0;

        rudder = new PhysicsEngineForFun.AeroForceControlOnRigidBody(
            new PhysicsEngineForFun.Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, 0, 0, 0, 0.01f, 0, 0),
            new PhysicsEngineForFun.Matrix3(0, 0, 0, 0, 0, 0, -0.01f, 0, 0),
            new PhysicsEngineForFun.Vector3(2.0f, 0.5f, 0),
            PhysicsEngineWindSpeed);
        rudder.controlSetting = 0f;
        ResetAircraft();
       
        PhysicsEngineForFun.Matrix3 i = new PhysicsEngineForFun.Matrix3();
        i.SetBlockInertiaTensor(new PhysicsEngineForFun.Vector3(2, 1, 1), 1);

        aircraft.SetMass(2.5f);
        aircraft.SetInertiaTensor(i);
        aircraft.linearDamping = aircraft.angularDamping = 0.8f;
        aircraft.angularDamping = 0.8f;
        aircraft.acceleration = new PhysicsEngineForFun.Vector3(0, -9.8f, 0);
        aircraft.CalculateDerivedData();

        registry.Add(aircraft, left_wing);
        registry.Add(aircraft, right_wing);
        registry.Add(aircraft, rudder);
        registry.Add(aircraft, tail);

       
    }

    private void FixedUpdate()
    {
        PhysicsEngineWindSpeed.x = windSpeed.x;
        PhysicsEngineWindSpeed.y = windSpeed.y;
        PhysicsEngineWindSpeed.z = windSpeed.z;

        aircraft.ClearAccumulator();
        PhysicsEngineForFun.Vector3 propulsion = new PhysicsEngineForFun.Vector3(-10.0f, 0, 0);
        propulsion = aircraft.transformMatrix.TransformDirection(propulsion);

        aircraft.AddForce(propulsion);

        registry.UpdateForces(0.02f);

        aircraft.Integrate(0.02f);

        if (aircraft.position.y < 0) aircraft.position.y = 0;
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.Q))
        {
            rudder_control += 0.1f;
        }
        else if (Input.GetKey(KeyCode.E))
        {
            rudder_control -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.W))
        {
            left_wing_control -= 0.1f;
            right_wing_control -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            left_wing_control += 0.1f;
            right_wing_control += 0.1f;
        }
        else if (Input.GetKey(KeyCode.D))
        {
            left_wing_control -= 0.1f;
            right_wing_control += 0.1f;
        }
        else if (Input.GetKey(KeyCode.A))
        {
            left_wing_control += 0.1f;
            right_wing_control -= 0.1f;
        }
        else if (Input.GetKey(KeyCode.X))
        {
            left_wing_control = 0.0f;
            right_wing_control = 0.0f;
            rudder_control = 0.0f;
        }
        else if (Input.GetKey(KeyCode.R))
        {
            ResetAircraft(); 
        }

        if (left_wing_control < -1.0f) left_wing_control = -1.0f;
        else if (left_wing_control > 1.0f) left_wing_control = 1.0f;
        if (right_wing_control < -1.0f) right_wing_control = -1.0f;
        else if (right_wing_control > 1.0f) right_wing_control = 1.0f;
        if (rudder_control < -1.0f) rudder_control = -1.0f;
        else if (rudder_control > 1.0f) rudder_control = 1.0f;

        left_wing.controlSetting = left_wing_control;
        right_wing.controlSetting = right_wing_control;
        rudder.controlSetting = rudder_control;
    }

    private void LateUpdate()
    {
        Vector3 pos = transform.position = aircraft.position.GetUnityEngineVector3();
        transform.localEulerAngles = aircraft.rotation.GetUnityEngineVector3();
        PhysicsEngineForFun.Vector3 offset = new PhysicsEngineForFun.Vector3(4.0f + aircraft.velocity.GetUnityEngineVector3().magnitude, 0, 0);
        offset = aircraft.transformMatrix.TransformDirection(offset);
        Camera.main.transform.position = new Vector3(pos.x + 4.0f + offset.x, pos.y + 5.0f, pos.z + offset.z);
        Vector3 forward = pos - Camera.main.transform.position;
        forward.Normalize();
        if (forward == Vector3.zero) return;
        Camera.main.transform.rotation = Quaternion.LookRotation(forward);

    }

    private void OnDestroy()
    {
        if(left_wing != null)
        {
            left_wing.Dispose();
            left_wing = null;
        }

        if (right_wing != null)
        {
            right_wing.Dispose();
            right_wing = null;
        }

        if(rudder != null)
        {
            rudder.Dispose();
            rudder = null;
        }

        if(tail != null)
        {
            tail.Dispose();
            tail = null;
        }

        if(aircraft != null)
        {
            aircraft.Dispose();
        }

        if(registry != null)
        { 
            registry.Dispose();
        }

        if(PhysicsEngineWindSpeed != null)
        {
            PhysicsEngineWindSpeed.Dispose();
        }
    }

    private void ResetAircraft()
    {
        aircraft.position = new PhysicsEngineForFun.Vector3(0, 0, 0);
        aircraft.velocity = new PhysicsEngineForFun.Vector3(0, 0, 0);
        aircraft.rotation = new PhysicsEngineForFun.Vector3(0, 0, 0);
        aircraft.orientation = new PhysicsEngineForFun.Quaternion(1, 0, 0, 0);
    }

    
}
