#include "chair.h"
#include "vertex1.h"

Chair::Chair(IDirect3DDevice9* device)
{
	// save a ptr to the device
	_device = device;


	-device->CreateVertexBuffer(//给板凳设置132个顶点存储空间
		132 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);
	/*
	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);
	*/
	Vertex* vertices;
	_vb->Lock(0, 0, (void**)&vertices, 0);
	// build box       

	// fill in vertex data

	// 顶点
	//板凳的左脚正面 
	vertices[0] = Vertex(-2.0f, -5.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[1] = Vertex(-2.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[2] = Vertex(-1.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[3] = Vertex(-1.0f, -5.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//0 1 2,0 2 3
	//板凳的左脚背面
	vertices[4] = Vertex(-2.0f, -1.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[5] = Vertex(-1.0f, -1.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[6] = Vertex(-1.0f, -5.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[7] = Vertex(-2.0f, -5.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);//5 4 7,5 7 6
	//板凳的左脚左面
	vertices[8] = Vertex(-2.0f, 0.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[9] = Vertex(-2.0f, 0.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[10] = Vertex(-2.0f, -5.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[11] = Vertex(-2.0f, -5.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//8 9 10,8 10 11
	//板凳的左脚右面
	vertices[12] = Vertex(-1.0f, -1.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[13] = Vertex(-1.0f, -1.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[14] = Vertex(2.0f, 0.0f, -2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[15] = Vertex(2.0f, -1.0f, -2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//12 13 14,12 14 15
	//板凳的左脚低面
	vertices[16] = Vertex(-2.0f, -5.0f, -2.0, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[17] = Vertex(-1.0f, -5.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[18] = Vertex(-1.0f, -5.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[19] = Vertex(-2.0f, -5.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//17 16 19,17 19 18
	////板凳的左脚后正面
	vertices[20] = Vertex(-2.0f, -1.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[21] = Vertex(-1.0f, -1.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[22] = Vertex(-1.0f, -5.0f, 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[23] = Vertex(-1.0f, -1.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//20 21 22, 20 22 23
	//板凳的左脚后背面
	vertices[24] = Vertex(-2.0f, -1.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[25] = Vertex(-1.0f, -1.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[26] = Vertex(-1.0f, -5.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[27] = Vertex(-2.0f, -5.0f, 3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//25 24 27,25 27 26
	//板凳的左脚后左面
	vertices[28] = Vertex(-2.0f, -1.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[29] = Vertex(-2.0f, -1.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[30] = Vertex(-2.0f, -5.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[31] = Vertex(-2.0f, -5.0f, 2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//29 28 31,29 31 30
	//板凳的左脚后右面
	vertices[32] = Vertex(-1.0f, -1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[33] = Vertex(-1.0f, -1.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[34] = Vertex(-1.0f, -5.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[35] = Vertex(-1.0f, -5.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//32 33 34,32 34 35
	//板凳的左脚后底面
	vertices[36] = Vertex(-2.0f, -5.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[37] = Vertex(-1.0f, -5.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[38] = Vertex(-1.0f, -5.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[39] = Vertex(-2.0f, -5.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//37 36 39,37 39 38
	//板凳的右脚正面
	vertices[40] = Vertex(1.0f, -5.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[41] = Vertex(1.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[42] = Vertex(2.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[43] = Vertex(2.0f, -5.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//41 42 43,41 43 40 
	//板凳的右脚背面
	vertices[44] = Vertex(1.0f, -5.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[45] = Vertex(1.0f, -1.0f, -2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[46] = Vertex(2.0f, -1.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[47] = Vertex(2.0f, -5.0f, -2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);//46 45 44,46 44 47
	////板凳的右脚左面
	vertices[48] = Vertex(1.0f, -1.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[49] = Vertex(1.0f, -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[50] = Vertex(1.0f, -5.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[51] = Vertex(1.0f, -5.0f, -3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//49 48 51,49 51 50
	////板凳的右脚右面
	vertices[52] = Vertex(2.0f, 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[53] = Vertex(2.0f, 0.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[54] = Vertex(2.0f, -5.0f, -2.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[55] = Vertex(2.0f, -5.0f, -3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//52 53 54,52 54 55
	////板凳的右脚底面
	vertices[56] = Vertex(1.0f, -5.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[57] = Vertex(2.0f, -5.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[58] = Vertex(2.0f, -5.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[59] = Vertex(1.0f, -5.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//57 56 59,57 59 58
	//板凳的后右脚正面
	vertices[60] = Vertex(1.0f, -5.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[61] = Vertex(1.0f, -1.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[62] = Vertex(2.0f, -1.0f, 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[63] = Vertex(2.0f, -5.0f, 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//61 62 63,61 63 60 
	//板凳的后右脚背面
	vertices[64] = Vertex(1.0f, -5.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[65] = Vertex(1.0f, -1.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[66] = Vertex(2.0f, -1.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[67] = Vertex(2.0f, -5.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);//66 65 64,66 64 67
	//板凳的后右脚左面
	vertices[68] = Vertex(1.0f, -5.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[69] = Vertex(1.0f, -1.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[70] = Vertex(1.0f, -1.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[71] = Vertex(1.0f, -5.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//70 69 68,70 68 71
	//板凳的后右脚右面
	vertices[72] = Vertex(2.0f, -5.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[73] = Vertex(2.0f, -1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[74] = Vertex(2.0f, -1.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[75] = Vertex(2.0f, -5.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//72 73 74,72 74 75
	//板凳的后右脚底面
	vertices[76] = Vertex(1.0f, -5.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[77] = Vertex(1.0f, -5.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[78] = Vertex(2.0f, -5.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[79] = Vertex(2.0f, -5.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//77 76 79,77 79 78
	//板凳的座位正面
	vertices[80] = Vertex(-1.0f, -1.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[81] = Vertex(-1.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[82] = Vertex(1.0f, 0.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[83] = Vertex(1.0f, -1.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//81 82 83,81 83 80 
	//板凳的座位背面
	vertices[84] = Vertex(-2.0f, -1.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[85] = Vertex(-2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[86] = Vertex(2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[87] = Vertex(2.0f, -1.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);//86 85 84,86 84 87
	//板凳的座位左面
	vertices[88] = Vertex(-2.0f, -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[89] = Vertex(-2.0f, 0.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[90] = Vertex(-2.0f, 0.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[91] = Vertex(-2.0f, -1.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//89 88 91,89 91 90
	//板凳的座位右面
	vertices[92] = Vertex(2.0f, -1.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[93] = Vertex(2.0f, 0.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[94] = Vertex(2.0f, 0.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[95] = Vertex(2.0f, -1.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//92 93 94,92 94 95
	//板凳的座位上面
	vertices[96] = Vertex(-2.0f, 0.0f, -3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[97] = Vertex(-2.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[98] = Vertex(2.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[99] = Vertex(2.0f, 0.0f, -3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);//97 98 99,97 99 96
	//板凳的座位底面
	vertices[100] = Vertex(-1.0f, -1.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[101] = Vertex(-1.0f, -1.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[102] = Vertex(1.0f, -1.0f, -2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[103] = Vertex(1.0f, -1.0f, -3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//102 101 100,102 100 103

	vertices[104] = Vertex(-2.0f, -1.0f, -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[105] = Vertex(-2.0f, -1.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[106] = Vertex(2.0f, -1.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[107] = Vertex(2.0f, -1.0f, -2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//106 105 104,106 104 107

	vertices[108] = Vertex(-1.0f, -1.0f, 2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	vertices[109] = Vertex(-1.0f, -1.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	vertices[110] = Vertex(1.0f, -1.0f, 3.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
	vertices[111] = Vertex(1.0f, -1.0f, 2.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);//110 109 108,110 108 111
	//板凳的凳背正面
	vertices[112] = Vertex(-2.0f, 0.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vertices[113] = Vertex(-2.0f, 5.0f, 2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	vertices[114] = Vertex(2.0f, 5.0f, 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	vertices[115] = Vertex(2.0f, 0.0f, 2.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);//112 113 114,112 114 115 
	//板凳的座位背面
	vertices[116] = Vertex(-2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vertices[117] = Vertex(-2.0f, 5.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	vertices[118] = Vertex(2.0f, 5.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	vertices[119] = Vertex(2.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);//118 117 116,118 116 119
	//板凳的座位左面
	vertices[120] = Vertex(-2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[121] = Vertex(-2.0f, 5.0f, 2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[122] = Vertex(-2.0f, 5.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[123] = Vertex(-2.0f, 0.0f, 3.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//122 121 120,122 120 123
	//板凳的座位右面
	vertices[124] = Vertex(2.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[125] = Vertex(2.0f, 5.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	vertices[126] = Vertex(2.0f, 5.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	vertices[127] = Vertex(2.0f, 0.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);//124 125 126,124 126 127
	//板凳的座位顶面
	vertices[128] = Vertex(-2.0f, 5.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	vertices[129] = Vertex(-2.0f, 5.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	vertices[130] = Vertex(2.0f, 5.0f, 3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	vertices[131] = Vertex(2.0f, 5.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);//128 129 130,128 130 131
	_vb->Unlock();


	_device->CreateIndexBuffer(//定义198个索引点
		198 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* indices = 0;
	_ib->Lock(0, 0, (void**)&indices, 0);
	// 索引点构成各个面
	//周面
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	indices[6] = 5; indices[7] = 4; indices[8] = 7;
	indices[9] = 5; indices[10] = 7; indices[11] = 6;
	indices[12] = 8; indices[13] = 9; indices[14] = 10;
	indices[15] = 8; indices[16] = 10; indices[17] = 11;
	indices[18] = 12; indices[19] = 13; indices[20] = 14;
	indices[21] = 12; indices[22] = 14; indices[23] = 15;
	indices[24] = 17; indices[25] = 16; indices[26] = 19;
	indices[27] = 17; indices[28] = 19; indices[29] = 18;
	indices[30] = 20; indices[31] = 21; indices[32] = 22;
	indices[33] = 20; indices[34] = 22; indices[35] = 23;
	indices[36] = 25; indices[37] = 24; indices[38] = 27;
	indices[39] = 25; indices[40] = 27; indices[41] = 26;
	indices[42] = 29; indices[43] = 28; indices[44] = 31;
	indices[45] = 29; indices[46] = 31; indices[47] = 30;
	indices[48] = 32; indices[49] = 33; indices[50] = 34;
	indices[51] = 32; indices[52] = 34; indices[53] = 35;
	indices[54] = 37; indices[55] = 36; indices[56] = 39;
	indices[57] = 37; indices[58] = 39; indices[59] = 38;
	indices[60] = 41; indices[61] = 42; indices[62] = 43;
	indices[63] = 41; indices[64] = 43; indices[65] = 42;
	indices[66] = 46; indices[67] = 45; indices[68] = 44;
	indices[69] = 46; indices[70] = 44; indices[71] = 47;
	indices[72] = 49; indices[73] = 48; indices[74] = 51;
	indices[75] = 49; indices[76] = 51; indices[77] = 50;
	indices[78] = 52; indices[79] = 53; indices[80] = 54;
	indices[81] = 52; indices[82] = 54; indices[83] = 55;
	indices[84] = 57; indices[85] = 56; indices[86] = 59;
	indices[87] = 57; indices[88] = 59; indices[89] = 58;
	indices[90] = 61; indices[91] = 62; indices[92] = 63;
	indices[93] = 61; indices[94] = 63; indices[95] = 60;
	indices[96] = 66; indices[97] = 65; indices[98] = 64;
	indices[99] = 66; indices[100] = 64; indices[101] = 67;
	indices[102] = 70; indices[103] = 69; indices[104] = 68;
	indices[105] = 70; indices[106] = 68; indices[107] = 71;
	indices[108] = 72; indices[109] = 73; indices[110] = 74;
	indices[111] = 72; indices[112] = 74; indices[113] = 75;
	indices[114] = 77; indices[115] = 76; indices[116] = 79;
	indices[117] = 77; indices[118] = 79; indices[119] = 78;
	indices[120] = 81; indices[121] = 82; indices[122] = 83;
	indices[123] = 81; indices[124] = 83; indices[125] = 80;
	indices[126] = 86; indices[127] = 85; indices[128] = 84;
	indices[129] = 86; indices[130] = 84; indices[131] = 87;
	indices[132] = 89; indices[133] = 88; indices[134] = 91;
	indices[135] = 89; indices[136] = 91; indices[137] = 90;
	indices[138] = 92; indices[139] = 93; indices[140] = 94;
	indices[141] = 92; indices[142] = 94; indices[143] = 95;
	indices[144] = 97; indices[145] = 98; indices[146] = 99;
	indices[147] = 97; indices[148] = 99; indices[149] = 96;
	indices[150] = 102; indices[151] = 101; indices[152] = 100;
	indices[153] = 102; indices[154] = 100; indices[155] = 103;
	indices[156] = 106; indices[157] = 105; indices[158] = 104;
	indices[159] = 106; indices[160] = 104; indices[161] = 107;
	indices[162] = 110; indices[163] = 109; indices[164] = 108;
	indices[165] = 110; indices[166] = 108; indices[167] = 111;
	indices[168] = 112; indices[169] = 113; indices[170] = 114;
	indices[171] = 112; indices[172] = 114; indices[173] = 115;
	indices[174] = 118; indices[175] = 117; indices[176] = 116;
	indices[177] = 118; indices[178] = 116; indices[179] = 119;
	indices[180] = 122; indices[181] = 121; indices[182] = 120;
	indices[183] = 122; indices[184] = 120; indices[185] = 123;
	indices[186] = 124; indices[187] = 125; indices[188] = 126;
	indices[189] = 124; indices[190] = 126; indices[191] = 127;
	indices[192] = 128; indices[193] = 129; indices[194] = 130;
	indices[195] = 128; indices[196] = 130; indices[197] = 131;


	_ib->Unlock();
}

Chair::~Chair()
{
	if (_vb){ _vb->Release(); _vb = 0; }
	if (_ib){ _ib->Release(); _ib = 0; }
}

bool Chair::draw7(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
{
	if (world)
		_device->SetTransform(D3DTS_WORLD, world);
	if (mtrl)
		_device->SetMaterial(mtrl);
	if (tex)
		_device->SetTexture(0, tex);

	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);
	_device->SetFVF(FVF_VERTEX);
	_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		132,
		0,
		66);

	return true;
}