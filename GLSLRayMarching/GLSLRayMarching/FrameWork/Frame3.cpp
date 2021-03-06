#include "Frame3.h"

Frame3::Frame3()
	: parent(0)
	, localTransform(Matrix4::Identity)
	, globalTransform(Matrix4::Identity)
	, validGlobal(false)
	, globalTransformInverse(Matrix4::Identity)
	, validGlobalInverse(false)
{
}

Frame3::~Frame3()
{
}

// init
void Frame3::SetZero()
{
	localTransform.SetZero();

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetIdentity()
{
	localTransform.SetIdentity();

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetTranslate(float x, float y, float z)
{
	localTransform.SetTranslate(x, y, z);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateX(float angle)
{
	localTransform.SetRotateX(angle);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateY(float angle)
{
	localTransform.SetRotateY(angle);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateZ(float angle)
{
	localTransform.SetRotateZ(angle);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateZXY(float z, float x, float y)
{
	localTransform.SetRotateZXY(z, x, y);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateZYX(float z, float y, float x)
{
	localTransform.SetRotateZYX(z, y, x);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetRotateAxisAngle(const Vector3& axis, float angle)
{
	localTransform.SetRotateAxisAngle(axis, angle);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetScale(float scale)
{
	localTransform.SetScale(scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetScale(float x, float y, float z)
{
	localTransform.SetScale(x, y, z);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetTranslateRotZXYScale(float tx, float ty, float tz, float rz, float rx, float ry, float scale)
{
	localTransform.SetTranslateRotZXYScale(tx, ty, tz, rz, rx, ry, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetTranslateRotZYXScale(float tx, float ty, float tz, float rz, float ry, float rx, float scale)
{
	localTransform.SetTranslateRotZYXScale(tx, ty, tz, rz, ry, rx, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetTranslateRotateAxisAngleScale(float tx, float ty, float tz, const Vector3& axis, const float angle, float scale)
{
	localTransform.SetTranslateRotateAxisAngleScale(tx, ty, tz, axis, angle, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetTranslateScale(float tx, float ty, float tz, float scale)
{
	localTransform.SetTranslateScale(tx, ty, tz, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetLookAt(const Vector3& position, const Vector3& objective, const Vector3& up)
{
	localTransform.SetLookAt(position, objective, up);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetLookAtScale(const Vector3& position, const Vector3& objective, const Vector3& up, float scale)
{
	localTransform.SetLookAtScale(position, objective, up, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetStandOn(const Vector3& position, const Vector3& objective, const Vector3& up)
{
	localTransform.SetStandOn(position, objective, up);

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetStandOnScale(const Vector3& position, const Vector3& objective, const Vector3& up, float scale)
{
	localTransform.SetStandOnScale(position, objective, up, scale);

	InValidateGlobal();
	InValidateGlobalInverse();
}

// setter
void Frame3::SetLocalTransform(const Matrix4& localtransform_)
{
	localTransform = localtransform_;

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetLocalPosition(const Vector3& localposition_)
{
	localTransform[0][3] = localposition_.X();
	localTransform[1][3] = localposition_.Y();
	localTransform[2][3] = localposition_.Z();

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetGlobalTransform(const Matrix4& globalTransform_)
{
	if (GetParent())
		localTransform = GetParent()->GetGlobalTransform().Inverse() * globalTransform_;
	else
		localTransform = globalTransform_;

	InValidateGlobal();
	InValidateGlobalInverse();
}

void Frame3::SetGlobalPosition(const Vector3& globalposition_)
{
	Vector3 localposition;
	if (GetParent())
		localposition = GetParent()->GetGlobalTransform().Inverse() * globalposition_;
	else
		localposition = globalposition_;

	localTransform[0][3] = localposition.X();
	localTransform[1][3] = localposition.Y();
	localTransform[2][3] = localposition.Z();

	InValidateGlobal();
	InValidateGlobalInverse();
}

// getter
const Matrix4& Frame3::GetLocalTransform() const
{
	return localTransform;
}

Vector3 Frame3::GetLocalPosition() const
{
	return Vector3(localTransform[0][3],
		localTransform[1][3],
		localTransform[2][3]);
}

Vector3 Frame3::GetLocalXAxis() const
{
	return Vector3(localTransform[0][0],
		localTransform[1][0],
		localTransform[2][0]);
}

Vector3 Frame3::GetLocalYAxis() const
{
	return Vector3(localTransform[0][1],
		localTransform[1][1],
		localTransform[2][1]);
}

Vector3 Frame3::GetLocalZAxis() const
{
	return Vector3(localTransform[0][2],
		localTransform[1][2],
		localTransform[2][2]);
}

const Matrix4& Frame3::GetGlobalTransform()
{
	ValidateGlobal();

	return globalTransform;
}

Vector3 Frame3::GetGlobalPosition()
{
	ValidateGlobal();

	return Vector3(globalTransform[0][3],
		globalTransform[1][3],
		globalTransform[2][3]);
}

Vector3 Frame3::GetGlobalXAxis()
{
	ValidateGlobal();

	return Vector3(globalTransform[0][0],
		globalTransform[1][0],
		globalTransform[2][0]);
}

Vector3 Frame3::GetGlobalYAxis()
{
	ValidateGlobal();

	return Vector3(globalTransform[0][1],
		globalTransform[1][1],
		globalTransform[2][1]);
}

Vector3 Frame3::GetGlobalZAxis()
{
	ValidateGlobal();

	return Vector3(globalTransform[0][2],
		globalTransform[1][2],
		globalTransform[2][2]);
}

Matrix4 Frame3::GetInverseGlobalTransform()
{
	ValidateGlobal();
	ValidateGlobalInverse();
	
	return globalTransformInverse;
}

void Frame3::ValidateGlobal()
{
	if (validGlobal)
		return;

	if (parent)
		globalTransform = parent->GetGlobalTransform() * localTransform;
	else
		globalTransform = localTransform;

	validGlobal = true;
}

void Frame3::InValidateGlobal()
{
	validGlobal = false;
	for (int i = 0; i < GetNumChildren(); i++)
	{
		GetChild(i)->InValidateGlobal();
	}
}

bool Frame3::IsValidGlobal()
{
	return validGlobal;
}

void Frame3::ValidateGlobalInverse()
{
	if( validGlobalInverse )
		return;

	globalTransformInverse = GetGlobalTransform().Inverse();
	validGlobalInverse = true;
}

void Frame3::InValidateGlobalInverse()
{
	validGlobalInverse = false;
	for (int i=0; i<children.size(); i++)
	{
		children[i]->InValidateGlobalInverse();
	}
}

bool Frame3::IsValidGlobalInverse()
{
	return validGlobalInverse;
}

// setter
void Frame3::AddChild(Frame3* child_)
{
	Assert(child_->GetParent() == nullptr); // not child already

	children.push_back(child_);
	child_->parent = this;
	child_->InValidateGlobal();
	//child_->InValidateGlobalInverse();
}

void Frame3::SetChild(Frame3* child_, int index_)
{
	Assert(child_->GetParent() == 0); // not child already
	Assert(index_ >= 0 && index_ < children.size()); // index in range

	RemoveChild(index_);

	InsertChild(child_, index_);
}

void Frame3::InsertChild(Frame3* child_, int index_)
{
	Assert(child_->GetParent() == nullptr); // not child already

	auto iterator = std::find(children.begin(), children.end(), child_);
	if (iterator != children.end())
	{
		children.insert(iterator, child_);
		child_->parent = this;
		child_->InValidateGlobal();
	}
}

void Frame3::RemoveAllChildren()
{
	while (children.size() > 0)
	{
		RemoveChild(0);
	};
}

void Frame3::RemoveChild(int index_)
{
	Assert(index_ >= 0 && index_ < children.size());

	RemoveChild(children[index_]);
}

void Frame3::RemoveChild(Frame3* child_)
{
	auto iterator = std::find(children.begin(), children.end(), child_);
	if (iterator != children.end())
	{
		child_->parent = nullptr;
		child_->InValidateGlobal();

		children.erase(iterator);
	}
}

// getter
int Frame3::GetAllChildren(std::vector<Frame3*>& children_)
{
	children_.resize(children.size());
	for (int i = 0; i < children.size(); i++)
	{
		children_[i] = children[i];
	}

	return children.size();
}

Frame3* Frame3::GetChild(int index_) const
{
	Assert(index_ >= 0 && index_ < children.size());

	return children[index_];
}

int Frame3::IndexOfChild(Frame3* child_) const
{
	for (int i = 0; i < children.size(); i++)
	{
		if (child_ == children[i])
			return i;
	}

	return -1;
}

int Frame3::GetNumChildren() const
{
	return children.size();
}

Frame3* Frame3::GetParent()
{
	return parent;
}

const Frame3* Frame3::GetParent() const
{
	return parent;
}