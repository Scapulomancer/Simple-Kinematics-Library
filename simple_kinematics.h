
/*
* SKL: Simple Kinematics Library
*/


/*
* 3d vector with operators and rotation functions implemented.
* Axes are assumed to be normalised. All angles are in radians.
*/

struct Vec3f
{
	float x,y,z;
	inline
	Vec3f operator+(Vec3f v)
	{
		Vec3f result;
		result.x=x+v.x;
		result.y=y+v.y;
		result.z=z+v.z;
		return result;
	}
	inline
	Vec3f operator-(Vec3f v)
	{
		Vec3f result;
		result.x=x-v.x;
		result.y=y-v.y;
		result.z=z-v.z;
		return result;
	}
	inline
	void operator=(Vec3f v)
	{
		x=v.x;
		y=v.y;
		z=v.z;
	}
	inline
	float mag()
	{
		return sqrt(x*x+y*y+z*z);
	}
	inline
	Vec3f norm()
	{
		if(x==0.0f&&y==0.0f&&z==0.0f) return *this;
		Vec3f result{x/mag(),y/mag(),z/mag()};
		return result;
	}
	inline
	Vec3f operator*(float s)
	{
		Vec3f result={x*s,y*s,z*s};
		return result;
	}
	inline
	float operator*(Vec3f v)
	{
		return v.x*x+v.y*y+v.z*z;
	}
	inline
	Vec3f operator^(Vec3f v)
	{
		Vec3f result={y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x};
		return result;
	}
	inline
	void operator+=(Vec3f v)
	{
		x+=v.x;
		y+=v.y;
		z+=v.z;
	}
	inline
	void operator-=(Vec3f v)
	{
		x-=v.x;
		y-=v.y;
		z-=v.z;
	}
	inline
	void operator*=(Vec3f v)
	{
		x*=v.x;
		y*=v.y;
		z*=v.z;
	}
	inline
	void operator/=(Vec3f v)
	{
		x/=v.x;
		y/=v.y;
		z/=v.z;
	}
	inline
	void operator+=(float s)
	{
		x+=s;
		y+=s;
		z+=s;
	}
	inline
	void operator-=(float s)
	{
		x-=s;
		y-=s;
		z-=s;
	}
	inline
	void operator*=(float s)
	{
		x*=s;
		y*=s;
		z*=s;
	}
	inline
	void operator/=(float s)
	{
		x/=s;
		y/=s;
		z/=s;
	}
	inline
	Vec3f turn(float angle, Vec3f axis)
	{
		Vec3f result;
		float c=cos(angle),s=sin(angle);
		result.x = x*(c+axis.x*axis.x*(1.f-c)) + y*(axis.x*axis.y*(1.f-c)-axis.z*s) + z*(axis.x*axis.z*(1.f-c)+axis.y*s);
		result.y = y*(c+axis.y*axis.y*(1.f-c)) + z*(axis.y*axis.z*(1.f-c)-axis.x*s) + x*(axis.y*axis.x*(1.f-c)+axis.z*s);
		result.z = z*(c+axis.z*axis.z*(1.f-c)) + x*(axis.z*axis.x*(1.f-c)-axis.y*s) + y*(axis.z*axis.y*(1.f-c)+axis.x*s);
		return result;
	}
	inline
	void turnInPlace(float angle,Vec3f axis)
	{
		float c=cos(angle),s=sin(angle);
		float nx = x*(c+axis.x*axis.x*(1.f-c)) + y*(axis.x*axis.y*(1.f-c)-axis.z*s) + z*(axis.x*axis.z*(1.f-c)+axis.y*s);
		float ny = y*(c+axis.y*axis.y*(1.f-c)) + z*(axis.y*axis.z*(1.f-c)-axis.x*s) + x*(axis.y*axis.x*(1.f-c)+axis.z*s);
		float nz = z*(c+axis.z*axis.z*(1.f-c)) + x*(axis.z*axis.x*(1.f-c)-axis.y*s) + y*(axis.z*axis.y*(1.f-c)+axis.x*s);
		x=nx;
		y=ny;
		z=nz;
	}
	inline
	Vec3f rotate(Vec3f rot);
	inline
	Vec3f inverseRotate(Vec3f rot);
	inline
	void rotateInPlace(Vec3f rot);
	inline
	void inverseRotateInPlace(Vec3f rot);
	inline
	Vec3f rotateExtrinsic(Vec3f rot,Vec3f xAxis,Vec3f yAxis,Vec3f zAxis)
	{
		Vec3f result=turn(rot.x,xAxis);
		result.turnInPlace(rot.y,yAxis);
		result.turnInPlace(rot.z,zAxis);
		return result;
	}
	inline
	Vec3f rotateIntrinsic(Vec3f rot,Vec3f xAxis,Vec3f yAxis,Vec3f zAxis)
	{
		Vec3f result=turn(rot.x,xAxis);
		result.turnInPlace(rot.y,yAxis.turn(rot.x,xAxis));
		result.turnInPlace(rot.z,zAxis.turn(rot.y,yAxis.turn(rot.x,xAxis)));
		return result;
	}
};
const Vec3f X_AXIS={1,0,0},Y_AXIS={0,1,0},Z_AXIS={0,0,1};
inline
Vec3f operator*(float s,Vec3f v)
{
	Vec3f result={v.x*s,v.y*s,v.z*s};
	return result;
}
inline
Vec3f Vec3f::rotate(Vec3f rot)
{
	Vec3f result=*this;
	result.rotateInPlace(rot);
	return result;
}
inline
Vec3f Vec3f::inverseRotate(Vec3f rot)
{
	Vec3f result=*this;
	result.inverseRotateInPlace(rot);
	return result;
}
inline
void Vec3f::rotateInPlace(Vec3f rot)
{
	turnInPlace(rot.x,X_AXIS);
	turnInPlace(rot.y,Y_AXIS);
	turnInPlace(rot.z,Z_AXIS);
}
inline
void Vec3f::inverseRotateInPlace(Vec3f rot)
{
	turnInPlace(-rot.z,Z_AXIS);
	turnInPlace(-rot.y,Y_AXIS);
	turnInPlace(-rot.x,X_AXIS);
}
Vec3f vec3f(float x,float y,float z)
{
	Vec3f v={x,y,z};
	return v;
}
Vec3f vec3f()
{
	Vec3f v={0,0,0};
	return v;
}


/*
* Operator that applies a rotation and displacement to a vector or another link transform.
* Can only operate on vectors to the right.
*/
struct LinkTransform
{
	Vec3f displacement,rotation;
	Vec3f xAxis,yAxis,zAxis;
	
	void init()
	{
		xAxis=X_AXIS;
		yAxis=Y_AXIS;
		zAxis=Z_AXIS;
		rotation={0.f,0.f,0.f};
		displacement={0.f,0.f,0.f};
	}
	
	void init(Vec3f rot,Vec3f disp)
	{
		rotation=rot;
		displacement=disp;
		updateAxes();
	}
	
	/*
	* Determines axes from set of rotations.
	*/
	void updateAxes()
	{
		xAxis={1.f,0.f,0.f};
		yAxis={0.f,1.f,0.f};
		zAxis={0.f,0.f,1.f};
		
		xAxis.rotateInPlace(rotation);
		yAxis.rotateInPlace(rotation);
		zAxis.rotateInPlace(rotation);
	}
	
	Vec3f operator*(Vec3f v)
	{
		Vec3f result=xAxis*v.x+yAxis*v.y+zAxis*v.z+displacement;
		return result;
	}
	
	Vec3f operator/(Vec3f v)
	{
		return inverse()*v;
	}
	
	LinkTransform operator=(LinkTransform t)
	{
		displacement=t.displacement;
		rotation=t.rotation;
		xAxis=t.xAxis;
		yAxis=t.yAxis;
		zAxis=t.zAxis;
	}
	
	LinkTransform operator*(LinkTransform t)
	{
		LinkTransform result;
		result.displacement=displacement+t.displacement.x*xAxis+t.displacement.y*yAxis+t.displacement.z*zAxis;
		
		result.xAxis=t.xAxis.x*xAxis+t.xAxis.y*yAxis+t.xAxis.z*zAxis;
		result.yAxis=t.yAxis.x*xAxis+t.yAxis.y*yAxis+t.yAxis.z*zAxis;
		result.zAxis=t.zAxis.x*xAxis+t.zAxis.y*yAxis+t.zAxis.z*zAxis;
		
		result.determineRotation();

		return result;
	}
	
	/*
	* Return inverse transform.
	*/
	LinkTransform inverse()
	{
		LinkTransform result;
		result.xAxis={xAxis.x,yAxis.x,zAxis.x};
		result.yAxis={xAxis.y,yAxis.y,zAxis.y};
		result.zAxis={xAxis.z,yAxis.z,zAxis.z};
		result.displacement={-(displacement*xAxis),-(displacement*yAxis),-(displacement*zAxis)};
		
		result.determineRotation();
		
		return result;
	}
	
	LinkTransform operator/(LinkTransform t)
	{
		return inverse()*t;
	}
	
	/*
	* Works backwards from current axes to find a set of rotations that would produce them.
	*/
	void determineRotation()
	{
		Vec3f xa=xAxis,ya=yAxis,za=zAxis;
		
		rotation.z=-atan2(xa.y,sqrt(pow(xa.x,2)+pow(xa.z,2)));
		xa.turnInPlace(-rotation.z,Z_AXIS);
		ya.turnInPlace(-rotation.z,Z_AXIS);
		za.turnInPlace(-rotation.z,Z_AXIS);
		
		rotation.y=-atan2(xa.z,xa.x);
		xa.turnInPlace(-rotation.y,Y_AXIS);
		ya.turnInPlace(-rotation.y,Y_AXIS);
		za.turnInPlace(-rotation.y,Y_AXIS);
		
		rotation.x=-atan2(ya.z,ya.y);
		xa.turnInPlace(-rotation.x,X_AXIS);
		ya.turnInPlace(-rotation.x,X_AXIS);
		za.turnInPlace(-rotation.x,X_AXIS);
		
	}
	
	/*
	* For debugging. Writes current axes into string.
	*/
	void printAxes(char* str)
	{
		sprintf(str,"(%f %f %f) (%f %f %f) (%f %f %f)",xAxis.x,xAxis.y,xAxis.z,yAxis.x,yAxis.y,yAxis.z,zAxis.x,zAxis.y,zAxis.z);
	}
};

/*
* Node in the kinematic hierarchy. Multiple can be linked together using 'parent' pointer to create model.
* 'rel' represents the reletive transform in the parents frame and must be set.
* 'tot' represents the total transform and is calculated in 'update()'. This should not be altered manually.
*/
struct Bone
{
	LinkTransform rel,tot;
	Vec3f maxRot,minRot,relCentre;
	Bone *parent;
	int label;
	bool boundsSet;
	
	void init()
	{
		parent=NULL;
		rel.init();
		tot.init();
		boundsSet=false;
	}
	void checkBounds()
	{
		if(!boundsSet) return;
		
		if(rel.rotation.x>maxRot.x) rel.rotation.x=maxRot.x;
		else if(rel.rotation.x<minRot.x) rel.rotation.x=minRot.x;
		if(rel.rotation.y>maxRot.y) rel.rotation.y=maxRot.y;
		else if(rel.rotation.y<minRot.y) rel.rotation.y=minRot.y;
		if(rel.rotation.z>maxRot.z) rel.rotation.z=maxRot.z;
		else if(rel.rotation.z<minRot.z) rel.rotation.z=minRot.z;
	}
	void update()
	{
		rel.updateAxes();
		if(parent) tot=parent->tot*rel;
		else tot=rel;
	}
	Vec3f getCentre()
	{
		return tot*relCentre;
	}
};

/*
* Container for kinematic hierarchy with bones indexed.
* 'init' must be called once.
* Use 'initBone' to initialise bones by index. If 'parent' is less than 0, the bone will be initialised with no parent.
* Bones can be initialied in any order but the index of a bone must be greater than that of its parent.
* Once bones have been set, call 'update' to perform forward calculation.
*/
struct Skeleton
{
	int noBones,*parents;
	Bone *bones;
	void init(int _noBones)
	{
		noBones=_noBones;
		parents=(int*)malloc(noBones*sizeof(int));
		bones=(Bone*)malloc(noBones*sizeof(Bone));
	}
	void uninit()
	{
		free(parents);
		free(bones);
		parents=NULL;
		bones=NULL;
	}
	void initBone(int bone,int parent,Vec3f disp,Vec3f rot)
	{
		if(bone<0||bone>=noBones||parent>=noBones)
		{
			printf("Bone index out of bounds!");
			return;
		}
		
		if(parent>=bone)
		{
			printf("Parent index must be lower than bone index!");
			return;
		}
		
		if(parent<0)
		{
			bones[bone].parent=NULL;
			parents[bone]=-1;
		}
		else
		{
			bones[bone].parent=&bones[parent];
			parents[bone]=parent;
		}
		
		bones[bone].rel.displacement=disp;
		bones[bone].rel.rotation=rot;
	}
	void setBounds(int bone,Vec3f minRot,Vec3f maxRot)
	{
		bones[bone].minRot=minRot;
		bones[bone].maxRot=maxRot;
		bones[bone].boundsSet=true;
	}
	void checkBounds()
	{
		for(int bone=0;bone<noBones;bone++) if(bones[bone].boundsSet) bones[bone].checkBounds();
	}
	void update()
	{
		for(int bone=0;bone<noBones;bone++) bones[bone].update();
	}
};
