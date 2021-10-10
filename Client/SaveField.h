#pragma once
class CSaveField
{
public:
	CSaveField();
	~CSaveField();

public:
	void Initialize();
	void LateInit();
	void Update();
	void LateUpdate();
	void Render();
	void Release();
};

