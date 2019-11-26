let main = () => {
  /* TODO refactor: extract _initShader, _render func? */

  let canvas = DomExtend.querySelector(DomExtend.document, "#webgl");

  let gl =
    Gl.getWebgl1Context(
      canvas,
      {
        "alpha": true,
        "depth": true,
        "stencil": false,
        "antialias": true,
        "premultipliedAlpha": true,
        "preserveDrawingBuffer": false,
      }: Gl.contextConfigJsObj,
    );

  let program1 =
    gl |> Gl.createProgram |> Utils.initShader(GLSL.vs1, GLSL.fs1, gl);

  let program2 =
    gl |> Gl.createProgram |> Utils.initShader(GLSL.vs2, GLSL.fs2, gl);

  let (vertices1, indices1) = Utils.createTriangleGeometryData();
  let (vertices2, indices2) = Utils.createTriangleGeometryData();
  let (vertices3, indices3) = Utils.createTriangleGeometryData();

  let vaoExt = GPUDetect.unsafeGetVAOExt(gl);

  let vao1 = Utils.initVAO((vertices1, indices1), program1, vaoExt, gl);

  let vao2 = Utils.initVAO((vertices2, indices2), program2, vaoExt, gl);

  let vao3 = Utils.initVAO((vertices3, indices3), program1, vaoExt, gl);

  let vMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.setLookAt((0., 0.0, 5.), (0., 0., (-100.)), (0., 1., 0.));

  let pMatrix =
    Matrix.createIdentityMatrix()
    |> Matrix.buildPerspective((
         30.,
         (canvas##width |> Js.Int.toFloat)
         /. (canvas##height |> Js.Int.toFloat),
         1.,
         100.,
       ));

  Gl.clearColor(0., 0., 0., 1., gl);

  /* TODO extract state to store data */
  let _loopBody = () => {
    Gl.clear(Gl.getColorBufferBit(gl) lor Gl.getDepthBufferBit(gl), gl);

    Gl.enable(Gl.getDepthTest(gl), gl);

    Gl.enable(Gl.getCullFace(gl), gl);
    Gl.cullFace(Gl.getBack(gl), gl);

    Gl.useProgram(program1, gl);

    Utils.sendAttributeData(vao1, vaoExt);

    Utils.sendCameraUniformData((vMatrix, pMatrix), program1, gl);

    Utils.sendModelUniformData1(
      (
        Matrix.createIdentityMatrix() |> Matrix.setTranslation((0.75, 0., 0.)),
        (1., 0., 0.),
      ),
      program1,
      gl,
    );

    Gl.drawElements(
      Gl.getTriangles(gl),
      indices1 |> Js.Typed_array.Uint16Array.length,
      Gl.getUnsignedShort(gl),
      0,
      gl,
    );
    Gl.useProgram(program2, gl);

    Utils.sendAttributeData(vao2, vaoExt);

    Utils.sendCameraUniformData((vMatrix, pMatrix), program2, gl);

    Utils.sendModelUniformData2(
      (
        Matrix.createIdentityMatrix()
        |> Matrix.setTranslation(((-0.), 0., 0.5)),
        (0., 0.8, 0.),
        (0., 0.5, 0.),
      ),
      program2,
      gl,
    );

    Gl.drawElements(
      Gl.getTriangles(gl),
      indices2 |> Js.Typed_array.Uint16Array.length,
      Gl.getUnsignedShort(gl),
      0,
      gl,
    );

    Gl.useProgram(program1, gl);

    Utils.sendAttributeData(vao3, vaoExt);

    Utils.sendCameraUniformData((vMatrix, pMatrix), program1, gl);

    Utils.sendModelUniformData1(
      (
        Matrix.createIdentityMatrix()
        |> Matrix.setTranslation(((-0.5), 0., (-2.))),
        (0., 0., 1.),
      ),
      program1,
      gl,
    );

    Gl.drawElements(
      Gl.getTriangles(gl),
      indices3 |> Js.Typed_array.Uint16Array.length,
      Gl.getUnsignedShort(gl),
      0,
      gl,
    );
  };

  let rec _loop = () =>
    DomExtend.requestAnimationFrame((time: float) => {
      _loopBody();

      _loop() |> ignore;
    });

  _loop();
};