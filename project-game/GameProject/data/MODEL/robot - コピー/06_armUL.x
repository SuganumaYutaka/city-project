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
 26;
 0.00000;0.00000;-1.50000;,
 0.00000;-1.29904;-0.75000;,
 14.00000;-1.29904;-0.75000;,
 14.00000;0.00000;-1.50000;,
 -0.00000;-1.29904;0.75000;,
 14.00000;-1.29904;0.75000;,
 0.00000;0.00000;1.50000;,
 14.00000;0.00000;1.50000;,
 0.00000;1.29903;0.75000;,
 14.00000;1.29904;0.75000;,
 0.00000;1.29903;-0.75000;,
 14.00000;1.29904;-0.75000;,
 0.00000;0.00000;-1.50000;,
 14.00000;0.00000;-1.50000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 0.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;,
 14.00000;0.00000;0.00000;;
 
 18;
 4;3,2,1,0;,
 4;2,5,4,1;,
 4;5,7,6,4;,
 4;7,9,8,6;,
 4;9,11,10,8;,
 4;11,13,12,10;,
 3;0,1,14;,
 3;1,4,15;,
 3;4,6,16;,
 3;6,8,17;,
 3;8,10,18;,
 3;10,12,19;,
 3;2,3,20;,
 3;5,2,21;,
 3;7,5,22;,
 3;9,7,23;,
 3;11,9,24;,
 3;13,11,25;;
 
 MeshMaterialList {
  6;
  18;
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4;;
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
   0.304314;0.304314;0.304314;1.000000;;
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
   0.765490;0.765490;0.765490;1.000000;;
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
  8;
  -1.000000;0.000000;0.000000;,
  0.000000;-0.499999;-0.866026;,
  0.000000;-1.000000;0.000000;,
  -0.000000;-0.499999;0.866026;,
  -0.000000;0.500001;0.866025;,
  -0.000001;1.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  -0.000000;0.500001;-0.866025;;
  18;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;7,7,7,7;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;6,6,6;;
 }
 MeshTextureCoords {
  26;
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.166670;1.000000;,
  0.000000;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.833330;0.000000;,
  0.833330;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.083330;0.000000;,
  0.250000;0.000000;,
  0.416670;0.000000;,
  0.583330;0.000000;,
  0.750000;0.000000;,
  0.916670;0.000000;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;;
 }
}
