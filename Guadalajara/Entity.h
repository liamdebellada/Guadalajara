#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#include "Offsets.h"

struct vec3 { float x, y, z; };

class Ent {
public:
	union {
		DEFINE_MEMBER_N(vec3, origin, hazedumper::netvars::m_vecOrigin);
		DEFINE_MEMBER_N(int, clientId, 0x64);
		DEFINE_MEMBER_N(int, m_iHealth, hazedumper::netvars::m_iHealth);
		DEFINE_MEMBER_N(vec3, m_vecViewOffset, hazedumper::netvars::m_vecViewOffset);
		DEFINE_MEMBER_N(int, m_iDefaultFOV, hazedumper::netvars::m_iDefaultFOV);
	};
};

class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(int hEnt) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual void* GetClientEntity(int entnum) = 0;
	virtual void* GetClientEntityFromHandle(int hEnt) = 0;

	// Returns number of entities currently in use
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

	// Returns highest index actually used
	virtual int					GetHighestEntityIndex(void) = 0;

	// Sizes entity list to specified size
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};