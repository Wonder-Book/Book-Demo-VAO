type vaoExt = {
  .
  [@bs.meth] "createVertexArrayOES": unit => Gl.vao,
  [@bs.meth] "bindVertexArrayOES": Js.Nullable.t(Gl.vao) => unit,
};

external glExtensionToVAOExt: Gl.extension => vaoExt = "%identity";

let unsafeGetVAOExt = gl =>
  switch (
    gl
    |> Gl.getExtension("OES_vertex_array_object")
    |> Js.Nullable.toOption
    |> Js.Option.map((. ext) => ext |> glExtensionToVAOExt)
  ) {
  | None => Utils.errorAndReturn("not support OES_vertex_array_object")
  | Some(vaoExt) => vaoExt
  };