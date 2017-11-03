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
 159;
 0.04091;21.40522;-6.00012;,
 4.28356;21.40522;-4.24277;,
 4.28356;8.93287;-4.24277;,
 0.04091;8.93287;-6.00012;,
 6.04091;21.40522;-0.00012;,
 6.04091;8.93287;-0.00012;,
 4.28356;21.40522;4.24253;,
 4.28356;8.93287;4.24253;,
 0.04091;21.40522;5.99988;,
 0.04091;8.93287;5.99988;,
 -4.28258;21.40522;4.24253;,
 -4.28258;8.93287;4.24253;,
 -6.03993;21.40522;-0.00012;,
 -6.03993;8.93287;-0.00012;,
 -4.28258;21.40522;-4.24277;,
 -4.28258;8.93287;-4.24277;,
 0.04091;21.40522;-6.00012;,
 0.04091;8.93287;-6.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;21.40522;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 0.04091;8.93287;-0.00012;,
 -0.07251;24.21630;-2.92898;,
 2.04881;24.21630;-2.05030;,
 2.04881;22.71630;-2.05030;,
 -0.07251;22.71630;-2.92898;,
 2.92749;24.21630;0.07102;,
 2.92749;22.71630;0.07102;,
 2.04881;24.21630;2.19234;,
 2.04881;22.71630;2.19234;,
 -0.07251;24.21630;3.07102;,
 -0.07251;22.71630;3.07102;,
 -2.19383;24.21630;2.19234;,
 -2.19383;22.71630;2.19234;,
 -3.07251;24.21630;0.07102;,
 -3.07251;22.71630;0.07102;,
 -2.19383;24.21630;-2.05030;,
 -2.19383;22.71630;-2.05030;,
 -0.07251;24.21630;-2.92898;,
 -0.07251;22.71630;-2.92898;,
 2.04881;21.21630;-2.05030;,
 -0.07251;21.21630;-2.92898;,
 2.92749;21.21630;0.07102;,
 2.04881;21.21630;2.19234;,
 -0.07251;21.21630;3.07102;,
 -2.19383;21.21630;2.19234;,
 -3.07251;21.21630;0.07102;,
 -2.19383;21.21630;-2.05030;,
 -0.07251;21.21630;-2.92898;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;24.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 -0.07251;21.21630;0.07102;,
 0.05102;10.16863;-4.50012;,
 3.23300;10.16863;-3.18210;,
 3.23300;4.16864;-3.18210;,
 0.05102;4.16864;-4.50012;,
 4.55102;10.16863;-0.00012;,
 4.55102;4.16864;-0.00012;,
 3.23300;10.16863;3.18186;,
 3.23300;4.16864;3.18186;,
 0.05102;10.16863;4.49988;,
 0.05102;4.16864;4.49988;,
 -3.23202;10.16863;3.18186;,
 -3.23202;4.16864;3.18186;,
 -4.55004;10.16863;-0.00012;,
 -4.55004;4.16864;-0.00012;,
 -3.23202;10.16863;-3.18210;,
 -3.23202;4.16864;-3.18210;,
 0.05102;10.16863;-4.50012;,
 0.05102;4.16864;-4.50012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;10.16863;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 0.05102;4.16864;-0.00012;,
 -5.87787;4.64688;-5.57986;,
 0.00000;4.64688;-5.57986;,
 0.00000;2.34715;-5.57986;,
 -8.08262;2.34715;-5.57986;,
 5.87787;4.64688;-5.57986;,
 8.08262;2.34715;-5.57986;,
 0.00000;0.04741;-5.57986;,
 -8.08262;0.04741;-5.57986;,
 8.08262;0.04741;-5.57986;,
 5.87787;4.64688;-5.57986;,
 8.08262;4.64688;0.00000;,
 8.08262;2.34715;0.00000;,
 8.08262;2.34715;-5.57986;,
 5.87787;4.64688;5.57986;,
 8.08262;2.34715;5.57986;,
 8.08262;0.04741;0.00000;,
 8.08262;0.04741;-5.57986;,
 8.08262;0.04741;5.57986;,
 5.87787;4.64688;5.57986;,
 0.00000;4.64688;5.57986;,
 0.00000;2.34715;5.57986;,
 8.08262;2.34715;5.57986;,
 -5.87787;4.64688;5.57986;,
 -8.08262;2.34715;5.57986;,
 0.00000;0.04741;5.57986;,
 8.08262;0.04741;5.57986;,
 -8.08262;0.04741;5.57986;,
 -5.87787;4.64688;5.57986;,
 -8.08262;4.64688;0.00000;,
 -8.08262;2.34715;0.00000;,
 -8.08262;2.34715;5.57986;,
 -5.87787;4.64688;-5.57986;,
 -8.08262;2.34715;-5.57986;,
 -8.08262;0.04741;0.00000;,
 -8.08262;0.04741;5.57986;,
 -8.08262;0.04741;-5.57986;,
 0.00000;4.64688;0.00000;,
 -8.08262;4.64688;0.00000;,
 8.08262;4.64688;0.00000;,
 0.00000;4.64688;-5.57986;,
 -5.87787;4.64688;-5.57986;,
 5.87787;4.64688;-5.57986;,
 -8.08262;0.04741;0.00000;,
 0.00000;0.04741;0.00000;,
 8.08262;0.04741;0.00000;,
 -8.08262;0.04741;-5.57986;,
 0.00000;0.04741;-5.57986;,
 8.08262;0.04741;-5.57986;;
 
 104;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;37,36,52,53;,
 4;36,39,54,52;,
 4;39,41,55,54;,
 4;41,43,56,55;,
 4;43,45,57,56;,
 4;45,47,58,57;,
 4;47,49,59,58;,
 4;49,51,60,59;,
 3;61,35,34;,
 3;62,38,35;,
 3;63,40,38;,
 3;64,42,40;,
 3;65,44,42;,
 3;66,46,44;,
 3;67,48,46;,
 3;68,50,48;,
 3;69,53,52;,
 3;70,52,54;,
 3;71,54,55;,
 3;72,55,56;,
 3;73,56,57;,
 3;74,57,58;,
 3;75,58,59;,
 3;76,59,60;,
 4;77,78,79,80;,
 4;78,81,82,79;,
 4;81,83,84,82;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 4;87,89,90,88;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 3;95,78,77;,
 3;96,81,78;,
 3;97,83,81;,
 3;98,85,83;,
 3;99,87,85;,
 3;100,89,87;,
 3;101,91,89;,
 3;102,93,91;,
 3;103,80,79;,
 3;104,79,82;,
 3;105,82,84;,
 3;106,84,86;,
 3;107,86,88;,
 3;108,88,90;,
 3;109,90,92;,
 3;110,92,94;,
 4;111,112,113,114;,
 4;112,115,116,113;,
 4;114,113,117,118;,
 4;113,116,119,117;,
 4;120,121,122,123;,
 4;121,124,125,122;,
 4;123,122,126,127;,
 4;122,125,128,126;,
 4;129,130,131,132;,
 4;130,133,134,131;,
 4;132,131,135,136;,
 4;131,134,137,135;,
 4;138,139,140,141;,
 4;139,142,143,140;,
 4;141,140,144,145;,
 4;140,143,146,144;,
 4;138,130,147,148;,
 4;130,124,149,147;,
 4;148,147,150,151;,
 4;147,149,152,150;,
 4;153,154,135,145;,
 4;154,155,128,135;,
 4;156,157,154,153;,
 4;157,158,155,154;;
 
 MeshMaterialList {
  1;
  104;
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
  49;
  0.000000;1.000000;0.000000;,
  0.003314;0.000000;-0.999995;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.003314;0.000000;0.999995;,
  -0.704759;0.000000;0.709447;,
  -1.000000;0.000000;0.000000;,
  -0.704759;0.000000;-0.709446;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.000000;0.000000;1.000000;,
  -0.707107;0.000000;0.707107;,
  -1.000000;0.000000;0.000000;,
  -0.707107;0.000000;-0.707107;,
  -0.707107;0.000000;0.707107;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.005466;0.000000;-0.999985;,
  0.707107;0.000000;-0.707107;,
  1.000000;0.000000;0.000000;,
  0.707107;0.000000;0.707107;,
  0.005466;0.000000;0.999985;,
  -0.703231;0.000000;0.710961;,
  -1.000000;0.000000;0.000000;,
  -0.703231;0.000000;-0.710961;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.930764;0.365621;0.000000;,
  0.982736;0.185015;0.000000;,
  1.000000;0.000000;0.000000;,
  0.920373;0.361540;0.149008;,
  0.000000;0.000000;1.000000;,
  0.979891;0.184480;0.076033;,
  -0.930764;0.365621;0.000000;,
  -0.982736;0.185015;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.920373;0.361540;-0.149008;,
  0.979891;0.184480;-0.076033;,
  -0.920373;0.361540;0.149008;,
  -0.979891;0.184480;0.076033;,
  -0.920373;0.361540;-0.149008;,
  -0.979891;0.184480;-0.076033;;
  104;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,16,19,15;,
  4;16,17,17,19;,
  4;17,18,18,17;,
  4;18,11,11,18;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;13,14,14,13;,
  4;14,15,15,14;,
  4;15,19,16,15;,
  4;19,17,17,16;,
  4;17,18,18,17;,
  4;18,11,11,18;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;10,10,10;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,25,25,24;,
  4;25,26,26,25;,
  4;26,27,27,26;,
  4;27,28,28,27;,
  4;28,29,29,28;,
  4;29,22,22,29;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;21,21,21;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  3;30,30,30;,
  4;31,31,31,31;,
  4;31,31,31,31;,
  4;31,31,31,31;,
  4;31,31,31,31;,
  4;43,32,33,44;,
  4;32,35,37,33;,
  4;44,33,34,34;,
  4;33,37,34,34;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;36,36,36,36;,
  4;45,38,39,46;,
  4;38,47,48,39;,
  4;46,39,40,40;,
  4;39,48,40,40;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;;
 }
 MeshTextureCoords {
  159;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
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
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
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
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;;
 }
}