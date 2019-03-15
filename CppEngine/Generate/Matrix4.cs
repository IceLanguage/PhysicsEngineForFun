//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.12
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class Matrix4 : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Matrix4(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Matrix4 obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~Matrix4() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PhysicsEngineForFunPINVOKE.delete_Matrix4(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public Matrix4() : this(PhysicsEngineForFunPINVOKE.new_Matrix4(), true) {
  }

  public float GetDeterminant() {
    float ret = PhysicsEngineForFunPINVOKE.Matrix4_GetDeterminant(swigCPtr);
    return ret;
  }

  public void SetOrientationAndPos(Quaternion q, Vector3 pos) {
    PhysicsEngineForFunPINVOKE.Matrix4_SetOrientationAndPos(swigCPtr, Quaternion.getCPtr(q), Vector3.getCPtr(pos));
    if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
  }

  public Vector3 Transform(Vector3 vector) {
    Vector3 ret = new Vector3(PhysicsEngineForFunPINVOKE.Matrix4_Transform(swigCPtr, Vector3.getCPtr(vector)), true);
    if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 TransformInverse(Vector3 vector) {
    Vector3 ret = new Vector3(PhysicsEngineForFunPINVOKE.Matrix4_TransformInverse(swigCPtr, Vector3.getCPtr(vector)), true);
    if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 TransformDirection(Vector3 vector) {
    Vector3 ret = new Vector3(PhysicsEngineForFunPINVOKE.Matrix4_TransformDirection(swigCPtr, Vector3.getCPtr(vector)), true);
    if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Vector3 TransformInverseDirection(Vector3 vector) {
    Vector3 ret = new Vector3(PhysicsEngineForFunPINVOKE.Matrix4_TransformInverseDirection(swigCPtr, Vector3.getCPtr(vector)), true);
    if (PhysicsEngineForFunPINVOKE.SWIGPendingException.Pending) throw PhysicsEngineForFunPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Matrix3 GetTransposeMatrix3() {
    Matrix3 ret = new Matrix3(PhysicsEngineForFunPINVOKE.Matrix4_GetTransposeMatrix3(swigCPtr), true);
    return ret;
  }

}
