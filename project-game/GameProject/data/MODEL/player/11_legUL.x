xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 86;
 2.73041;0.00612;0.00000;,
 2.62993;-2.03508;-2.04120;,
 2.62993;0.00612;-2.88669;,
 2.73041;0.00612;0.00000;,
 2.62993;-2.88058;-0.00000;,
 2.73041;0.00612;0.00000;,
 2.62993;-2.03508;2.04120;,
 2.73041;0.00612;0.00000;,
 2.62993;0.00612;2.88669;,
 2.73041;0.00612;0.00000;,
 2.62993;2.04731;2.04120;,
 2.73041;0.00612;0.00000;,
 2.62993;2.89281;0.00000;,
 2.73041;0.00612;0.00000;,
 2.62993;2.04731;-2.04120;,
 2.73041;0.00612;0.00000;,
 2.62993;0.00612;-2.88669;,
 -0.00835;-2.88058;-2.88669;,
 -0.00835;0.00612;-4.08240;,
 -0.00835;-4.07629;0.00000;,
 -0.00835;-2.88058;2.88669;,
 -0.00835;0.00612;4.08240;,
 -0.00835;2.89281;2.88669;,
 -0.00835;4.08851;0.00000;,
 -0.00835;2.89281;-2.88669;,
 -0.00835;0.00612;-4.08240;,
 -2.64662;-2.03508;-2.04120;,
 -2.64662;0.00612;-2.88669;,
 -2.64662;-2.88058;0.00000;,
 -2.64662;-2.03508;2.04120;,
 -2.64662;0.00612;2.88669;,
 -2.64662;2.04731;2.04120;,
 -2.64662;2.89281;0.00000;,
 -2.64662;2.04731;-2.04120;,
 -2.64662;0.00612;-2.88669;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 -2.61489;0.00612;0.00000;,
 0.00628;-1.25604;-2.24301;,
 1.55273;-1.25045;-1.60248;,
 1.55273;-6.74124;-1.55479;,
 0.00628;-6.74684;-2.19530;,
 2.19328;-1.23688;-0.05611;,
 2.19328;-6.72769;-0.00839;,
 1.55273;-1.22331;1.49026;,
 1.55273;-6.71412;1.53798;,
 0.00628;-1.21771;2.13080;,
 0.00628;-6.70852;2.17851;,
 -1.54017;-1.22331;1.49026;,
 -1.54017;-6.71412;1.53798;,
 -2.18072;-1.23688;-0.05611;,
 -2.18072;-6.72769;-0.00839;,
 -1.54017;-1.25045;-1.60248;,
 -1.54017;-6.74124;-1.55479;,
 0.00628;-1.25604;-2.24301;,
 0.00628;-6.74684;-2.19530;,
 1.55273;-12.23206;-1.50707;,
 0.00628;-12.23765;-2.14761;,
 2.19328;-12.21849;0.03932;,
 1.55273;-12.20492;1.58570;,
 0.00628;-12.19933;2.22623;,
 -1.54017;-12.20492;1.58570;,
 -2.18072;-12.21849;0.03932;,
 -1.54017;-12.23206;-1.50707;,
 0.00628;-12.23765;-2.14761;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-1.23688;-0.05611;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;,
 0.00628;-12.21849;0.03932;;
 
 64;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;2,1,17,18;,
 4;1,4,19,17;,
 4;4,6,20,19;,
 4;6,8,21,20;,
 4;8,10,22,21;,
 4;10,12,23,22;,
 4;12,14,24,23;,
 4;14,16,25,24;,
 4;18,17,26,27;,
 4;17,19,28,26;,
 4;19,20,29,28;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 3;27,26,35;,
 3;26,28,36;,
 3;28,29,37;,
 3;29,30,38;,
 3;30,31,39;,
 3;31,32,40;,
 3;32,33,41;,
 3;33,34,42;,
 4;43,44,45,46;,
 4;44,47,48,45;,
 4;47,49,50,48;,
 4;49,51,52,50;,
 4;51,53,54,52;,
 4;53,55,56,54;,
 4;55,57,58,56;,
 4;57,59,60,58;,
 4;46,45,61,62;,
 4;45,48,63,61;,
 4;48,50,64,63;,
 4;50,52,65,64;,
 4;52,54,66,65;,
 4;54,56,67,66;,
 4;56,58,68,67;,
 4;58,60,69,68;,
 3;70,44,43;,
 3;71,47,44;,
 3;72,49,47;,
 3;73,51,49;,
 3;74,53,51;,
 3;75,55,53;,
 3;76,57,55;,
 3;77,59,57;,
 3;78,62,61;,
 3;79,61,63;,
 3;80,63,64;,
 3;81,64,65;,
 3;82,65,66;,
 3;83,66,67;,
 3;84,67,68;,
 3;85,68,69;;
 
 MeshMaterialList {
  1;
  64;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\gray.jpg";
   }
  }
 }
 MeshNormals {
  82;
  1.000000;0.000000;0.000000;,
  0.999395;0.000000;-0.034787;,
  0.999395;-0.024598;-0.024598;,
  0.999395;-0.034787;-0.000000;,
  0.999395;-0.024598;0.024598;,
  0.999395;0.000000;0.034787;,
  0.999395;0.024598;0.024598;,
  0.999395;0.034787;0.000000;,
  0.999395;0.024598;-0.024598;,
  -0.000001;0.000001;-1.000000;,
  -0.000001;-0.707106;-0.707108;,
  -0.000000;-1.000000;0.000000;,
  -0.000001;-0.707106;0.707108;,
  -0.000001;0.000001;1.000000;,
  -0.000001;0.707107;0.707107;,
  -0.000001;1.000000;0.000000;,
  -0.000001;0.707107;-0.707107;,
  -0.412800;0.000001;-0.910822;,
  -0.412800;-0.644047;-0.644049;,
  -0.412801;-0.910821;0.000000;,
  -0.412800;-0.644047;0.644049;,
  -0.412800;0.000001;0.910822;,
  -0.412800;0.644048;0.644048;,
  -0.412800;0.910822;0.000000;,
  -0.412800;0.644048;-0.644048;,
  -1.000000;-0.000000;0.000000;,
  0.412799;0.000001;-0.910822;,
  0.412799;-0.644047;-0.644049;,
  0.412800;-0.910822;-0.000000;,
  0.412799;-0.644047;0.644049;,
  0.412799;0.000001;0.910822;,
  0.412799;0.644049;0.644049;,
  0.412798;0.910822;0.000000;,
  0.412799;0.644049;-0.644049;,
  -0.999940;-0.000000;0.010991;,
  -0.999940;0.007772;0.007772;,
  -0.999940;0.010991;0.000000;,
  -0.999940;0.007772;-0.007772;,
  -0.999940;-0.000000;-0.010991;,
  -0.999940;-0.007772;-0.007772;,
  -0.999940;-0.010991;0.000000;,
  -0.999940;-0.007772;0.007772;,
  0.000000;0.999962;-0.008769;,
  0.000000;-0.008687;-0.999962;,
  0.707105;-0.006143;-0.707082;,
  1.000000;0.000001;0.000002;,
  0.707107;0.006145;0.707080;,
  0.000000;0.008690;0.999962;,
  -0.707107;0.006145;0.707080;,
  -1.000000;0.000001;0.000002;,
  -0.707105;-0.006143;-0.707082;,
  0.000000;-0.008687;-0.999962;,
  0.707106;-0.006143;-0.707081;,
  1.000000;0.000000;0.000002;,
  0.707107;0.006145;0.707080;,
  0.000000;0.008690;0.999962;,
  -0.707107;0.006145;0.707080;,
  -1.000000;0.000000;0.000002;,
  -0.707106;-0.006143;-0.707081;,
  0.000000;-0.008688;-0.999962;,
  0.707107;-0.006144;-0.707080;,
  1.000000;0.000000;0.000002;,
  0.707107;0.006145;0.707080;,
  0.000000;0.008691;0.999962;,
  -0.707106;0.006145;0.707080;,
  -1.000000;0.000000;0.000002;,
  -0.707107;-0.006144;-0.707080;,
  0.000000;-0.999962;0.008768;,
  0.000007;0.999962;-0.008769;,
  0.000000;0.999962;-0.008762;,
  0.000000;0.999961;-0.008775;,
  -0.000005;0.999962;-0.008769;,
  0.000000;0.999962;-0.008765;,
  0.000005;0.999962;-0.008769;,
  -0.000007;0.999962;-0.008769;,
  0.000000;-0.999962;0.008761;,
  -0.000007;-0.999962;0.008768;,
  0.000000;-0.999961;0.008775;,
  0.000007;-0.999962;0.008768;,
  0.000000;-0.999962;0.008761;,
  -0.000007;-0.999962;0.008768;,
  0.000007;-0.999962;0.008768;;
  64;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,1,8;,
  4;26,27,10,9;,
  4;27,28,11,10;,
  4;28,29,12,11;,
  4;29,30,13,12;,
  4;30,31,14,13;,
  4;31,32,15,14;,
  4;32,33,16,15;,
  4;33,26,9,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  3;34,35,25;,
  3;35,36,25;,
  3;36,37,25;,
  3;37,38,25;,
  3;38,39,25;,
  3;39,40,25;,
  3;40,41,25;,
  3;41,34,25;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,43,51,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,51,59,66;,
  3;42,68,69;,
  3;42,70,68;,
  3;42,71,70;,
  3;42,72,71;,
  3;42,73,72;,
  3;42,70,73;,
  3;42,74,70;,
  3;42,69,74;,
  3;67,75,76;,
  3;67,76,77;,
  3;67,77,78;,
  3;67,78,79;,
  3;67,79,80;,
  3;67,80,77;,
  3;67,77,81;,
  3;67,81,75;;
 }
 MeshTextureCoords {
  86;
  0.062500;0.000000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.187500;0.000000;,
  0.250000;0.250000;,
  0.312500;0.000000;,
  0.375000;0.250000;,
  0.437500;0.000000;,
  0.500000;0.250000;,
  0.562500;0.000000;,
  0.625000;0.250000;,
  0.687500;0.000000;,
  0.750000;0.250000;,
  0.812500;0.000000;,
  0.875000;0.250000;,
  0.937500;0.000000;,
  1.000000;0.250000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.375000;0.000000;,
  0.375000;0.500000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.625000;0.000000;,
  0.625000;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
