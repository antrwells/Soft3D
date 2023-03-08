#pragma once
class AppState
{
public:

	virtual void Init() {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void RenderAfter3D() {};

private:
};

