//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class ParticleContact : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal ParticleContact(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(ParticleContact obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~ParticleContact() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PhysicsEngineForFunPINVOKE.delete_ParticleContact(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public ParticleContact(Particle a, Particle b) : this(PhysicsEngineForFunPINVOKE.new_ParticleContact(Particle.getCPtr(a), Particle.getCPtr(b)), true) {
  }

  public void Resolve(float duration) {
    PhysicsEngineForFunPINVOKE.ParticleContact_Resolve(swigCPtr, duration);
  }

  public float CalculateSeparatingVelocity() {
    float ret = PhysicsEngineForFunPINVOKE.ParticleContact_CalculateSeparatingVelocity(swigCPtr);
    return ret;
  }

  public float restitutionCoefficient {
    set {
      PhysicsEngineForFunPINVOKE.ParticleContact_restitutionCoefficient_set(swigCPtr, value);
    } 
    get {
      float ret = PhysicsEngineForFunPINVOKE.ParticleContact_restitutionCoefficient_get(swigCPtr);
      return ret;
    } 
  }

  public float penetrationDepth {
    set {
      PhysicsEngineForFunPINVOKE.ParticleContact_penetrationDepth_set(swigCPtr, value);
    } 
    get {
      float ret = PhysicsEngineForFunPINVOKE.ParticleContact_penetrationDepth_get(swigCPtr);
      return ret;
    } 
  }

  public Vector3 contactNormal {
    set {
      PhysicsEngineForFunPINVOKE.ParticleContact_contactNormal_set(swigCPtr, Vector3.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = PhysicsEngineForFunPINVOKE.ParticleContact_contactNormal_get(swigCPtr);
      Vector3 ret = (cPtr == global::System.IntPtr.Zero) ? null : new Vector3(cPtr, false);
      return ret;
    } 
  }

}
