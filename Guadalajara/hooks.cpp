#include "hooks.h"

Hooks::CreateMoveFn originalFn;

float DotProduct(const Vector& a, const Vector& b);
void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
int counter = 0;
void test(QAngle vOldAngles, CUserCmd* cmd, float fOldForward, float fOldSidemove);
float DEG2RAD(float degrees);

bool __stdcall hkCreateMove(float sampleTime, CUserCmd* cmd) {
	//pre anti-aim
	originalFn(sampleTime, cmd);
	QAngle oldAngles;
	Interfaces->EngineClient->GetViewAngles(oldAngles);

	//anti-aim
    if (menu::breakAngles) {
        if (counter >= 1000-menu::rodeoSpeed) {
            cmd->viewangles.y = (float)(rand() % 359);
            counter = 0;
        }
    }
    counter++;

	//	cmd->viewangles.y = 60.f;
	test(oldAngles, cmd, cmd->forwardmove, cmd->sidemove);
	return false;
}

void test(QAngle vOldAngles, CUserCmd* cmd, float fOldForward, float fOldSidemove) {
	// side/forward move correction
	float deltaView;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

float DotProduct(const Vector& a, const Vector& b) {
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

float DEG2RAD(float degrees) {
	return degrees * ( M_PI / 180.0 );
}

void inline SinCos(float radians, float* sine, float* cosine)
{
#if defined( _X360 )
	XMScalarSinCos(sine, cosine, radians);
#elif defined( PLATFORM_WINDOWS_PC32 )
	_asm
	{
		fld		DWORD PTR[radians]
		fsincos

		mov edx, DWORD PTR[cosine]
		mov eax, DWORD PTR[sine]

		fstp DWORD PTR[edx]
		fstp DWORD PTR[eax]
	}
#elif defined( PLATFORM_WINDOWS_PC64 )
	*sine = sin(radians);
	*cosine = cos(radians);
#elif defined( POSIX )
	register double __cosr, __sinr;
	__asm ("fsincos" : "=t" (__cosr), "=u" (__sinr) : "0" (radians));

	*sine = __sinr;
	*cosine = __cosr;
#endif
}

void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[0]), &sy, &cy);
	SinCos(DEG2RAD(angles[1]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);


	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}


void Hooks::init() {
    uintptr_t createMoveAddress = (uintptr_t)((*(void***)Interfaces->ClientMode)[24]);
    originalFn = (Hooks::CreateMoveFn)DetourFunction((PBYTE)createMoveAddress, (PBYTE)hkCreateMove);
}

