//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace PhysicsEngineForFun
{
    public class AeroForceOnRigidBody : IRigidBodyForceGenerator
    {
        private global::System.Runtime.InteropServices.HandleRef swigCPtr;

        internal AeroForceOnRigidBody(global::System.IntPtr cPtr, bool cMemoryOwn) : base(PhysicsEngineForFunPINVOKE.AeroForceOnRigidBody_SWIGUpcast(cPtr), cMemoryOwn)
        {
            swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
        }

        internal static global::System.Runtime.InteropServices.HandleRef getCPtr(AeroForceOnRigidBody obj)
        {
            return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
        }

        ~AeroForceOnRigidBody()
        {
            Dispose();
        }

        public override void Dispose()
        {
            lock (this)
            {
                if (swigCPtr.Handle != global::System.IntPtr.Zero)
                {
                    if (swigCMemOwn)
                    {
                        swigCMemOwn = false;
                        PhysicsEngineForFunPINVOKE.delete_AeroForceOnRigidBody(swigCPtr);
                    }
                    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
                }
                global::System.GC.SuppressFinalize(this);
                base.Dispose();
            }
        }

        public AeroForceOnRigidBody(Matrix3 tensor, Vector3 position, Vector3 windspeed) : this(PhysicsEngineForFunPINVOKE.new_AeroForceOnRigidBody(Matrix3.getCPtr(tensor), Vector3.getCPtr(position), Vector3.getCPtr(windspeed)), true)
        {
            if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
        }

        public override void UpdateForce(RigidBody body, float duration)
        {
            PhysicsEngineForFunPINVOKE.AeroForceOnRigidBody_UpdateForce(swigCPtr, RigidBody.getCPtr(body), duration);
        }

    }
}