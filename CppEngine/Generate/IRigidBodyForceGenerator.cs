//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class IRigidBodyForceGenerator : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal IRigidBodyForceGenerator(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(IRigidBodyForceGenerator obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~IRigidBodyForceGenerator() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PhysicsEngineForFunPINVOKE.delete_IRigidBodyForceGenerator(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public virtual void UpdateForce(RigidBody body, float duration) {
    if (SwigDerivedClassHasMethod("UpdateForce", swigMethodTypes0)) PhysicsEngineForFunPINVOKE.IRigidBodyForceGenerator_UpdateForceSwigExplicitIRigidBodyForceGenerator(swigCPtr, RigidBody.getCPtr(body), duration); else PhysicsEngineForFunPINVOKE.IRigidBodyForceGenerator_UpdateForce(swigCPtr, RigidBody.getCPtr(body), duration);
  }

  public IRigidBodyForceGenerator() : this(PhysicsEngineForFunPINVOKE.new_IRigidBodyForceGenerator(), true) {
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("UpdateForce", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateIRigidBodyForceGenerator_0(SwigDirectorUpdateForce);
    PhysicsEngineForFunPINVOKE.IRigidBodyForceGenerator_director_connect(swigCPtr, swigDelegate0);
  }

  private bool SwigDerivedClassHasMethod(string methodName, global::System.Type[] methodTypes) {
    global::System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, global::System.Reflection.BindingFlags.Public | global::System.Reflection.BindingFlags.NonPublic | global::System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(IRigidBodyForceGenerator));
    return hasDerivedMethod;
  }

  private void SwigDirectorUpdateForce(global::System.IntPtr body, float duration) {
    UpdateForce((body == global::System.IntPtr.Zero) ? null : new RigidBody(body, false), duration);
  }

  public delegate void SwigDelegateIRigidBodyForceGenerator_0(global::System.IntPtr body, float duration);

  private SwigDelegateIRigidBodyForceGenerator_0 swigDelegate0;

  private static global::System.Type[] swigMethodTypes0 = new global::System.Type[] { typeof(RigidBody), typeof(float) };
}
