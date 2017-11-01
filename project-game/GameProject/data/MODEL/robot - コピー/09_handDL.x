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
 40;
 0.01225;0.00203;-0.75000;,
 -0.69487;-0.70508;-0.75000;,
 2.13356;-3.53351;-0.75000;,
 2.84067;-2.82640;-0.75000;,
 -0.69487;-0.70508;-0.75000;,
 -0.69487;-0.70508;0.75000;,
 2.13356;-3.53351;0.75000;,
 2.13356;-3.53351;-0.75000;,
 -0.69487;-0.70508;0.75000;,
 0.01225;0.00203;0.75000;,
 2.84067;-2.82640;0.75000;,
 2.13356;-3.53351;0.75000;,
 0.01225;0.00203;0.75000;,
 0.01225;0.00203;-0.75000;,
 2.84067;-2.82640;-0.75000;,
 2.84067;-2.82640;0.75000;,
 -0.69487;-0.70508;-0.75000;,
 0.01225;0.00203;-0.75000;,
 2.84067;-2.82640;-0.75000;,
 2.13356;-3.53351;-0.75000;,
 2.17485;-3.57954;-0.75000;,
 2.88196;-4.28664;-0.75000;,
 5.71038;-1.45822;-0.75000;,
 5.00328;-0.75111;-0.75000;,
 2.88196;-4.28664;-0.75000;,
 2.88196;-4.28664;0.75000;,
 5.71038;-1.45822;0.75000;,
 5.71038;-1.45822;-0.75000;,
 2.88196;-4.28664;0.75000;,
 2.17485;-3.57954;0.75000;,
 5.00328;-0.75111;0.75000;,
 5.71038;-1.45822;0.75000;,
 2.17485;-3.57954;0.75000;,
 2.17485;-3.57954;-0.75000;,
 5.00328;-0.75111;-0.75000;,
 5.00328;-0.75111;0.75000;,
 2.88196;-4.28664;-0.75000;,
 2.17485;-3.57954;-0.75000;,
 5.00328;-0.75111;-0.75000;,
 5.71038;-1.45822;-0.75000;;
 
 12;
 4;3,2,1,0;,
 4;7,6,5,4;,
 4;11,10,9,8;,
 4;15,14,13,12;,
 4;17,16,5,12;,
 4;15,6,19,18;,
 4;23,22,21,20;,
 4;27,26,25,24;,
 4;31,30,29,28;,
 4;35,34,33,32;,
 4;37,36,25,32;,
 4;35,26,39,38;;
 
 MeshMaterialList {
  5;
  12;
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.640000;0.640000;0.640000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.420392;0.420392;0.420392;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.263529;0.263529;0.263529;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.160000;0.160000;0.160000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  10;
  0.000000;0.000000;-1.000000;,
  -0.707107;-0.707107;0.000000;,
  0.000000;0.000000;1.000000;,
  0.707107;-0.707107;-0.000000;,
  0.707108;0.707106;0.000000;,
  -0.707102;0.707112;0.000000;,
  0.707107;-0.707107;-0.000000;,
  -0.707107;0.707107;0.000000;,
  -0.707102;-0.707112;0.000000;,
  0.707112;0.707102;0.000000;;
  12;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  40;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
