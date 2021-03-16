#include "Quaternion.h"

const IQuaternion IQuaternion::Identity(1, 0, 0, 0);
const IQuaternion IQuaternion::Zero(0, 0, 0, 0);
const int IQuaternion::ms_iNext[3] = { 1, 2, 0 };

const Quaternion Quaternion::Identity(1.0f, 0.0f, 0.0f, 0.0f);
const Quaternion Quaternion::Zero(0.0f, 0.0f, 0.0f, 0.0f);
const int Quaternion::ms_iNext[3] = { 1, 2, 0 };

const DQuaternion DQuaternion::Identity(1.0, 0.0, 0.0, 0.0);
const DQuaternion DQuaternion::Zero(0.0, 0.0, 0.0, 0.0);
const int DQuaternion::ms_iNext[3] = { 1, 2, 0 };