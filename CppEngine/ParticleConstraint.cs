//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class ParticleConstraint : IParticleContactGenerator {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal ParticleConstraint(global::System.IntPtr cPtr, bool cMemoryOwn) : base(PhysicsEngineForFunPINVOKE.ParticleConstraint_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ParticleConstraint obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ParticleConstraint() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PhysicsEngineForFunPINVOKE.delete_ParticleConstraint(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public float GetCurrentLength() {
    float ret = PhysicsEngineForFunPINVOKE.ParticleConstraint_GetCurrentLength(swigCPtr);
    return ret;
  }

  public override bool AddContact(ParticleContact contact, uint limit) {
    bool ret = PhysicsEngineForFunPINVOKE.ParticleConstraint_AddContact(swigCPtr, ParticleContact.getCPtr(contact), limit);
    return ret;
  }

  public Particle particle {
    set {
      PhysicsEngineForFunPINVOKE.ParticleConstraint_particle_set(swigCPtr, Particle.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = PhysicsEngineForFunPINVOKE.ParticleConstraint_particle_get(swigCPtr);
      Particle ret = (cPtr == global::System.IntPtr.Zero) ? null : new Particle(cPtr, false);
      return ret;
    } 
  }

  public Vector3 anchorPoint {
    set {
      PhysicsEngineForFunPINVOKE.ParticleConstraint_anchorPoint_set(swigCPtr, Vector3.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = PhysicsEngineForFunPINVOKE.ParticleConstraint_anchorPoint_get(swigCPtr);
      Vector3 ret = (cPtr == global::System.IntPtr.Zero) ? null : new Vector3(cPtr, false);
      return ret;
    } 
  }

}
