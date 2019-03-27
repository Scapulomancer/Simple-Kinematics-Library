/*
* Uses Skeleton from SKL to implement an animated hand model.
*/
struct HandModel
{
	Skeleton skeleton;
	
	static const int dof=26;
	int hand;

	float state[dof],minState[dof],maxState[dof];	
	
	int noVertices,noQuads,*quads,*vertexLabels,*quadLabels,*adjLabels;
	Vec3f *vertices,*equivalentVertices;
	float *vertexWeights;
	
	void changeState(float _state[dof])
	{
		for(int i=0;i<dof;i++) state[i]=_state[i];
		update();
	}
	void changeStateFrustrum(float _state[dof])
	{
		state[0]=_state[2]*_state[0];
		state[1]=_state[2]*_state[1];
		for(int i=2;i<dof;i++) state[i]=_state[i];
		
		update();
	}
	void getStateFrustrum(float _state[dof])
	{
		_state[0]=state[0]/state[2];
		_state[1]=state[1]/state[2];
		for(int i=2;i<dof;i++) _state[i]=state[i];
	}
	
	//Sets bone variables according to state then updates skeleton.
	void update()
	{
		//palm
		skeleton.bones[0].rel.displacement.x=state[0];
		skeleton.bones[0].rel.displacement.y=state[1];
		skeleton.bones[0].rel.displacement.z=state[2];
		skeleton.bones[0].rel.rotation.x=state[3];
		skeleton.bones[0].rel.rotation.y=state[4];
		skeleton.bones[0].rel.rotation.z=state[5];
			
		//thumb
		skeleton.bones[1].rel.rotation.x=state[6];
		skeleton.bones[1].rel.rotation.y=state[7];
		skeleton.bones[2].rel.rotation.x=state[8];
		skeleton.bones[3].rel.rotation.x=state[9];
		
		//index
		skeleton.bones[4].rel.rotation.x=state[10];
		skeleton.bones[4].rel.rotation.y=state[11];
		skeleton.bones[5].rel.rotation.x=state[12];
		skeleton.bones[6].rel.rotation.x=state[13];	
		
		//middle
		skeleton.bones[7].rel.rotation.x=state[14];
		skeleton.bones[7].rel.rotation.y=state[15];
		skeleton.bones[8].rel.rotation.x=state[16];
		skeleton.bones[9].rel.rotation.x=state[17];	
		
		//ring
		skeleton.bones[10].rel.rotation.x=state[18];
		skeleton.bones[10].rel.rotation.y=state[19];
		skeleton.bones[11].rel.rotation.x=state[20];
		skeleton.bones[12].rel.rotation.x=state[21];	
		
		//pinky
		skeleton.bones[13].rel.rotation.x=state[22];
		skeleton.bones[13].rel.rotation.y=state[23];
		skeleton.bones[14].rel.rotation.x=state[24];
		skeleton.bones[15].rel.rotation.x=state[25];
		
		skeleton.checkBounds();
		skeleton.update();
	}
	
	void renderKeypoints()
	{
		for(int i=0;i<16;i++)
		{
			if(i==0) glColor3b(127,0,0);
			else glColor3b(96-((i-1)%3)*32,32+((i-1)%3)*32,32+((i-1)/5)*32);
			renderSphere(skeleton.bones[i].tot.displacement.x,skeleton.bones[i].tot.displacement.y,skeleton.bones[i].tot.displacement.z,.05f);
		}
	}

	void makeRight()
	{
		skeleton.init(16);
		hand=1;
		initParts(3.6f);
		loadMeshFromFile(3.6f);
	}
	
	void makeLeft()
	{
		skeleton.init(16);
		hand=0;
		initParts(-3.6f);
		loadMeshFromFile(-3.6f);
	}
	
	//Loads specific mesh from specific file. Will not work with any other file. Sorry...
	void loadMeshFromFile(float scaleFactor)
	{
		char* filename="models/painted_hand.ply";
		
		Vec3f startingPos[skeleton.noBones],startingAngles[skeleton.noBones];
		
		startingPos[0]={-.06660f,.23298f,.12224f};
		startingPos[1]={-.08684f,.11805f,.26081f};
		startingPos[2]={-.09603f,.08922f,.29021f};
		startingPos[3]={-.11111f,.03830f,.35527f};
		startingPos[4]={-.01834f,.14575f,.40919f};
		startingPos[5]={-.00369f,.13579f,.47922f};
		startingPos[6]={-.00964f,.13016f,.55318f};
		startingPos[7]={.00204f,.21834f,.41495f};
		startingPos[8]={.02086f,.21898f,.50085f};
		startingPos[9]={-.00151f,.21438f,.58075f};
		startingPos[10]={-.01497f,.27823f,.41398f};
		startingPos[11]={-.00333f,.28530f,.48124f};
		startingPos[12]={-.01106f,.28661f,.54511f};
		startingPos[13]={-.03125f,.33461f,.38951f};
		startingPos[14]={-.02359f,.35393f,.44818f};
		startingPos[15]={-.03314f,.36437f,.49445f};
		
		startingAngles[0]={-0.2f,0.0f,-M_PI/2};
		startingAngles[1]={0.2f,0.8f,-M_PI/2-0.8f};
		startingAngles[2]={0.3f,0.8f,-M_PI/2-0.8f};
		startingAngles[3]={0.3f,0.8f,-M_PI/2-0.8f};
		startingAngles[4]={-0.2f,0.1f,-M_PI/2};
		startingAngles[5]={-0.1f,0.1f,-M_PI/2};
		startingAngles[6]={0.1f,0.1f,-M_PI/2};
		startingAngles[7]={-0.2f,0.f,-M_PI/2};
		startingAngles[8]={0.2f,0.f,-M_PI/2};
		startingAngles[9]={0.f,0.f,-M_PI/2};
		startingAngles[10]={0.f,-0.1f,-M_PI/2};
		startingAngles[11]={0.f,-0.1f,-M_PI/2};
		startingAngles[12]={0.f,-0.1f,-M_PI/2};
		startingAngles[13]={0.f,-0.3f,-M_PI/2};
		startingAngles[14]={0.f,-0.3f,-M_PI/2};
		startingAngles[15]={0.f,-0.3f,-M_PI/2};
		
		int vInParts[skeleton.noBones];
		for(int i=0;i<skeleton.noBones;i++)
		{
			vInParts[i]=0;
			if(i>0) skeleton.bones[i].relCentre={0.f,0.f,0.f};
			
			startingPos[i]*=scaleFactor;
			if(i==0)
			{
				skeleton.bones[i].tot.displacement=startingPos[i];
				skeleton.bones[i].tot.rotation=startingAngles[i];
				skeleton.bones[i].tot.updateAxes();
				skeleton.bones[i].rel=skeleton.bones[i].tot;
			}
			else
			{
				skeleton.bones[i].tot.displacement=startingPos[i];
				skeleton.bones[i].tot.rotation=startingAngles[i];
				skeleton.bones[i].tot.updateAxes();
				skeleton.bones[i].rel=skeleton.bones[i].parent->tot/skeleton.bones[i].tot;
			}
		}
	
		FILE* file=fopen(filename,"r");
		char buf[64];
		while(strcmp(buf,"vertex")) fscanf(file,"%s",buf);
		fscanf(file,"%d",&noVertices);
		while(strcmp(buf,"face")) fscanf(file,"%s",buf);
		fscanf(file,"%d",&noQuads);
		
		vertices=(Vec3f*)malloc(noVertices*sizeof(Vec3f));
		equivalentVertices=(Vec3f*)malloc(noVertices*sizeof(Vec3f));
		vertexLabels=(int*)malloc(noVertices*sizeof(int));
		adjLabels=(int*)malloc(noVertices*sizeof(int));
		vertexWeights=(float*)malloc(noVertices*sizeof(int));
		quads=(int*)malloc(4*noQuads*sizeof(int));
		quadLabels=(int*)malloc(noQuads*sizeof(int));
		
		while(strcmp(buf,"end_header")) fscanf(file,"%s",buf);
		
		for(int i=0;i<noVertices;i++)
		{
			int r,g,b;
			float nx,ny,nz;
			fscanf(file,"%f %f %f",&vertices[i].x,&vertices[i].y,&vertices[i].z);
			
			fscanf(file,"%f %f %f",&nx,&ny,&nz);
			fscanf(file,"%d %d %d",&r,&g,&b);
			vertexLabels[i]=r/16;
			adjLabels[i]=g/16;
			vertexWeights[i]=(float)b/256;
			
			vertices[i]*=scaleFactor;
			vertices[i]=skeleton.bones[vertexLabels[i]].tot/vertices[i];
			
			vInParts[vertexLabels[i]]++;
			skeleton.bones[vertexLabels[i]].relCentre+=vertices[i];
			
		}
		
		for(int i=0;i<skeleton.noBones;i++) skeleton.bones[i].relCentre/=(float)vInParts[i];
		
		int theNumberFour;
		for(int i=0;i<noQuads;i++)
		{
			fscanf(file,"%d %d %d %d %d",&theNumberFour,&quads[4*i],&quads[4*i+1],&quads[4*i+2],&quads[4*i+3]);
			quadLabels[i]=vertexLabels[quads[4*i]];
			for(int j=1;j<4;j++) if(vertexLabels[quads[4*i+j]]>quadLabels[i]) quadLabels[i]=vertexLabels[quads[4*i+j]];
		}
		
		state[0]=startingPos[0].x;
		state[1]=startingPos[0].y;
		state[2]=startingPos[0].z;
		state[3]=startingAngles[0].x;
		state[4]=startingAngles[0].y;
		state[5]=startingAngles[0].z;
		for(int i=0;i<5;i++)
		{
			state[4*i+6]=startingAngles[i*3+1].x;
			state[4*i+7]=startingAngles[i*3+1].y;
			state[4*i+8]=startingAngles[i*3+2].x;
			state[4*i+9]=startingAngles[i*3+3].x;
		}
 	}
	
	void renderMesh()
	{
		for(int i=0;i<noQuads;i++)
		{
			if(quadLabels[i]==0) glColor3b(127,0,0);
			else glColor3b(96-((quadLabels[i]-1)%3)*32,32+((quadLabels[i]-1)%3)*32,32+((quadLabels[i]-1)/5)*32);

			glBegin(GL_QUADS);
				for(int j=0;j<4;j++)
				{
					Vec3f v=skeleton.bones[vertexLabels[quads[4*i+j]]].tot*vertices[quads[4*i+j]];
					glVertex3f(v.x,v.y,v.z);
				}
			glEnd();
		}
	}
	
	// Sets up kinematic model. Also defines minState and maxState.
	void initParts(float scaleFactor)
	{
		//palm
		skeleton.initBone(0,-1,vec3f(),vec3f());
		
		skeleton.bones[0].minRot={-M_PI,-M_PI,-M_PI};
		skeleton.bones[0].maxRot={M_PI,M_PI,M_PI};
		
		minState[3]=skeleton.bones[0].minRot.x;
		minState[4]=skeleton.bones[0].minRot.y;
		minState[5]=skeleton.bones[0].minRot.z;
		maxState[3]=skeleton.bones[0].maxRot.x;
		maxState[4]=skeleton.bones[0].maxRot.y;
		maxState[5]=skeleton.bones[0].maxRot.z;
		
		//thumb
		skeleton.initBone(1,0,vec3f(25.5559f*scaleFactor,-4.7252f*scaleFactor,17.0950f*scaleFactor),vec3f());
		skeleton.setBounds(1,vec3f(0.2f,0.0f,-0.8f),vec3f(1.2f,0.8f,-0.8f));
		
		minState[6]=skeleton.bones[1].minRot.x;
		minState[7]=skeleton.bones[1].minRot.y;
		maxState[6]=skeleton.bones[1].maxRot.x;
		maxState[7]=skeleton.bones[1].maxRot.y;
		
		skeleton.initBone(2,1,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 27.2800f*scaleFactor),vec3f());
		skeleton.setBounds(2,vec3f(0.0f,0.0f,0.0f),vec3f(0.8f,0.0f,0.0f));
		
		minState[8]=skeleton.bones[2].minRot.x;
		maxState[8]=skeleton.bones[2].maxRot.x;
		
		skeleton.initBone(3,2,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 22.0002f*scaleFactor),vec3f());
		skeleton.setBounds(3,vec3f(-0.1f,0.f,0.f),vec3f(0.8f,0.f,0.f));
		
		minState[9]=skeleton.bones[3].minRot.x;
		maxState[9]=skeleton.bones[3].maxRot.x;
		
		//index
		skeleton.initBone(4,0,vec3f(11.3060f*scaleFactor,51.9315f*scaleFactor,43.9460f*scaleFactor),vec3f());
		skeleton.setBounds(4,vec3f(-10*M_PI/180,-10*M_PI/180,0),vec3f(90*M_PI/180,30*M_PI/180,0));
		
		minState[10]=skeleton.bones[4].minRot.x;
		minState[11]=skeleton.bones[4].minRot.y;
		maxState[10]=skeleton.bones[4].maxRot.x;
		maxState[11]=skeleton.bones[4].maxRot.y;
		
		skeleton.initBone(5,4,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 30.8001f*scaleFactor),vec3f());
		skeleton.setBounds(5,vec3f(),vec3f(100*M_PI/180,0,0));
		
		minState[12]=skeleton.bones[5].minRot.x;
		maxState[12]=skeleton.bones[5].maxRot.x;
		
		skeleton.initBone(6,5,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 23.1006f*scaleFactor),vec3f());
		skeleton.setBounds(6,vec3f(-10*M_PI/180,0,0),vec3f(80*M_PI/180,0,0));
		
		minState[13]=skeleton.bones[6].minRot.x;
		maxState[13]=skeleton.bones[6].maxRot.x;
		
		//middle
		skeleton.initBone(7,0,vec3f(-9.3694f*scaleFactor,49.2714f*scaleFactor,54.7870f*scaleFactor),vec3f());
		skeleton.setBounds(7,vec3f(-10*M_PI/180,-20*M_PI/180,0),vec3f(90*M_PI/180,20*M_PI/180,0));
		
		minState[14]=skeleton.bones[7].minRot.x;
		minState[15]=skeleton.bones[7].minRot.y;
		maxState[14]=skeleton.bones[7].maxRot.x;
		maxState[15]=skeleton.bones[7].maxRot.y;
		
		skeleton.initBone(8,7,vec3f(0.0f*scaleFactor,0.0f*scaleFactor,36.2998f*scaleFactor),vec3f());
		skeleton.setBounds(8,vec3f(),vec3f(100*M_PI/180,0,0));
		
		minState[16]=skeleton.bones[8].minRot.x;
		maxState[16]=skeleton.bones[8].maxRot.x;
		
		skeleton.initBone(9,8,vec3f(0.0f*scaleFactor,0.0f*scaleFactor,24.2004f*scaleFactor),vec3f());
		skeleton.setBounds(9,vec3f(-10*M_PI/180,0,0),vec3f(80*M_PI/180,0,0));
		
		minState[17]=skeleton.bones[9].minRot.x;
		maxState[17]=skeleton.bones[9].maxRot.x;
		
		//ring
		skeleton.initBone(10,0,vec3f(-27.4010f*scaleFactor, 41.8008f*scaleFactor,59.1520f*scaleFactor),vec3f());
		skeleton.setBounds(10,vec3f(-10*M_PI/180,-30*M_PI/180,0),vec3f(90*M_PI/180,30*M_PI/180,0));
		
		minState[18]=skeleton.bones[10].minRot.x;
		minState[19]=skeleton.bones[10].minRot.y;
		maxState[18]=skeleton.bones[10].maxRot.x;
		maxState[19]=skeleton.bones[10].maxRot.y;
		
		skeleton.initBone(11,10,vec3f(0.0f*scaleFactor,0.0f*scaleFactor,32.4003f*scaleFactor),vec3f());
		skeleton.setBounds(11,vec3f(),vec3f(100*M_PI/180,0,0));
		
		minState[20]=skeleton.bones[11].minRot.x;
		maxState[20]=skeleton.bones[11].maxRot.x;
		
		skeleton.initBone(12,11,vec3f(0.0f*scaleFactor,0.0f*scaleFactor,20.9000f*scaleFactor),vec3f());
		skeleton.setBounds(12,vec3f(-10*M_PI/180,0,0),vec3f(80*M_PI/180,0,0));
		
		minState[21]=skeleton.bones[12].minRot.x;
		maxState[21]=skeleton.bones[12].maxRot.x;
		
		//pinky
		skeleton.initBone(13,0,vec3f(-45.1265f*scaleFactor,24.0856f*scaleFactor,61.7170f*scaleFactor),vec3f());
		skeleton.setBounds(13,vec3f(-10*M_PI/180,-60*M_PI/180,0),vec3f(90*M_PI/180,30*M_PI/180,0));
		
		minState[22]=skeleton.bones[13].minRot.x;
		minState[23]=skeleton.bones[13].minRot.y;
		maxState[22]=skeleton.bones[13].maxRot.x;
		maxState[23]=skeleton.bones[13].maxRot.y;
		
		skeleton.initBone(14,13,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 19.5609f*scaleFactor),vec3f());
		skeleton.setBounds(14,vec3f(),vec3f(100*M_PI/180,0,0));
		
		minState[24]=skeleton.bones[14].minRot.x;
		maxState[24]=skeleton.bones[14].maxRot.x;
		
		skeleton.initBone(15,14,vec3f(0.0f*scaleFactor, 0.0f*scaleFactor, 14.2993f*scaleFactor),vec3f());
		skeleton.setBounds(15,vec3f(-10*M_PI/180,0,0),vec3f(80*M_PI/180,0,0));
		
		minState[25]=skeleton.bones[15].minRot.x;
		maxState[25]=skeleton.bones[15].maxRot.x;
	}
};
