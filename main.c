#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <Windows.h>
#include <stdbool.h>
#include "Maps.h"

// Maps의 X, Y, Z 크기
int nMapX = 16;
int nMapY = 16;
int nMapZ = 7;						// 높이. 즉 2차원 map의 개수. 바닥(막힘) 포함

// 콘솔 창 크기
int nScreenWidth = 180;				// 콘솔 화면 너비 X (columns). 
int nScreenHeight = 60;				// 콘솔 화면 높이 Y (rows). 

// Player 초기화 정보
float fPlayerX = 2.0f;
float fPlayerY = 2.0f;
float fPlayerZ = 3.0f;				// Player의 시야 높이
float fPlayerA = 0.0f;				// Player의 시선. 시계 방향 180도: +3.14159. 3시 방향이 0
float fFOV_XY = 3.14159f / 3.0f;	// 좌우 방향 Field of View. 180도 / 3 = 60도
float fFOV_Z = 3.14159f / 3.0f;		// 상하 방향 Fov
float fDepth = 24.0f;				// 최대 렌더링 거리. 16*16*5 공간의 대각선 최대 길이가 23.~
float fSpeed = 0.025f;				// 이동 속도

int main(void)
{
	// Create Screen Buffer
	wchar_t* screen = (wchar_t*)malloc(nScreenWidth * nScreenHeight * sizeof(wchar_t));
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	COORD dwWriteCoord = { 0,0 };
	/*
	* cout/printf는 느리기 때문에 콘솔 버퍼를 가져와서 직접 작성해야 한다.
	* wchar_t: wide character. 2바이트 UTF-8(Unicode)로 문자를 표현
	* HANDLE: 운영체제가 관리하는 자원(마우스 커서, 콘솔 창 등)을 쓸 수 있는 임시 키
	* CreateConsoleScreenBuffer(): 새 콘솔 화면 버퍼에 대한 핸들 값 반환
	* SetConsoleActiveScreenBuffer(hConsole): hConsole을 현재 표시된 콘솔 화면 버퍼로 설정
	* DWORD: Double Word. 4 byte
	*/

	// screen 초기화
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
	{
		screen[i] = L' ';
	}

	wchar_t** maps = map1;	// 맵 선택

	while (1)	// 한 번 실행될 때마다 화면 한 장 생성(1 Frame)
	{
#pragma region KeyInput
		/*
		* GetAsyncKeyState(): 키의 상태를 알아오는 함수.
		* 이전에 누른 적이 없고 호출 시점에는 눌려 있는 상태: 반환 값이 0x8000
		*/
		// map 선택
		if (GetAsyncKeyState(0x31) & 0x8000)	// map1
			maps = map1;
		if (GetAsyncKeyState(0x32) & 0x8000)	// map2
			maps = map2;
		if (GetAsyncKeyState(0x33) & 0x8000)	// map3
			maps = map3;
		if (GetAsyncKeyState(0x34) & 0x8000)	// map4
			maps = map4;
		if (GetAsyncKeyState(0x35) & 0x8000)	// map5
			maps = map5;

		// 반시계 방향 회전
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			fPlayerA -= fSpeed * 0.75f;
		// 시계 방향 회전
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			fPlayerA += fSpeed * 0.75f;
		// 전진 & 물체의 제일 아랫단, 아래에서 두번째 단과 충돌 감지
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * fSpeed;
			fPlayerY += cosf(fPlayerA) * fSpeed;
			if (maps[nMapZ - 3][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * fSpeed;
				fPlayerY -= cosf(fPlayerA) * fSpeed;
			}
			if (maps[nMapZ - 2][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * fSpeed;
				fPlayerY -= cosf(fPlayerA) * fSpeed;
			}
		}
		// 후진 & 물체의 제일 아랫단, 아래에서 두번째 단과 충돌 감지
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed;
			fPlayerY -= cosf(fPlayerA) * fSpeed;
			if (maps[nMapZ - 3][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeed;
				fPlayerY += cosf(fPlayerA) * fSpeed;
			}
			if (maps[nMapZ - 2][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX += sinf(fPlayerA) * fSpeed;
				fPlayerY += cosf(fPlayerA) * fSpeed;
			}
		}
		// 좌진 & 물체의 제일 아랫단, 아래에서 두번째 단과 충돌 감지
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			fPlayerX -= sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			fPlayerY += cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			if (maps[nMapZ - 3][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX += sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
				fPlayerY -= cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			}
			if (maps[nMapZ - 2][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX += sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
				fPlayerY -= cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			}
		}
		// 우진 & 물체의 제일 아랫단, 아래에서 두번째 단과 충돌 감지
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			fPlayerX += sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			fPlayerY -= cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			if (maps[nMapZ - 3][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX -= sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
				fPlayerY += cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			}
			if (maps[nMapZ - 2][((int)fPlayerX * nMapX + (int)fPlayerY)] == '#')
			{
				fPlayerX -= sinf(3.14159f / 2.0f - fPlayerA) * fSpeed;
				fPlayerY += cosf(3.14159f / 2.0f - fPlayerA) * fSpeed;
			}
		}
#pragma endregion KeyInput

		// 콘솔 창 윗줄부터
		for (int nowY = 0; nowY < nScreenHeight; nowY++)
		{
			// 콘솔 창 왼쪽부터
			for (int nowX = 0; nowX < nScreenWidth; nowX++)
			{
				// XY평면에서 투영된 광선의 각도
				float fRayAngle_XY = (fPlayerA - fFOV_XY / 2.0f) + ((float)nowX / (float)nScreenWidth) * fFOV_XY;
				// 위아래 방향 각도
				float fRayAngle_Z = fFOV_Z / 2.0f - ((float)nowY / (float)nScreenHeight) * fFOV_Z;

				// ray casting의 증분 단위
				float fStepSize = 0.1f;
				// 위에서 구한 각도의 벽-플레이어 까지의 거리
				float fDistanceToWall = 0.0f;

				bool bHitWall = false;		// Set when ray hits wall block
				bool bBoundary = false;		// Set when ray hits boundary between two wall blocks
				bool isFloor = false;		// 바닥인지 확인

				// Ray의 단위 벡터당 X, Y, Z좌표값의 증가량
				float fEyeX = sinf(fRayAngle_XY) * cosf(fRayAngle_Z);
				float fEyeY = cosf(fRayAngle_XY) * cosf(fRayAngle_Z);
				float fEyeZ = sinf(fRayAngle_Z);

				// ray-casting 벽 만날때까지 진행
				// ray가 진행하다가 벽을 만나지 못할 경우가 있으므로 ray-casting 최대 범위 지정
				// Incrementally cast ray from player, along ray angle, testing for intersection with a block
				while (!bHitWall && fDistanceToWall < fDepth)
				{
					fDistanceToWall += fStepSize;	// 벽-플레이어 까지의 거리를 구하기 위해 0.1만큼 계속 더함

					// ray가 벽으로 fStepSize씩 가는 동안 map에서의 x, y, z위치
					int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
					int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
					int nTestZ = (int)(fPlayerZ + fEyeZ * fDistanceToWall);

					// ray가 맵의 밖에 위치하는 경우
					if (nTestX < 0 || nTestX >= nMapX || nTestY < 0 || nTestY >= nMapY || nTestZ < 0 || nTestZ >= nMapZ)
					{
						bHitWall = true;
						fDistanceToWall = fDepth;	// Just set distance to maximum depth
					}
					else
					{
						// ray가 맵의 벽에 닿은 경우
						if (maps[nMapZ - 1 - nTestZ][nTestX * nMapX + nTestY] == '#')
						{
							bHitWall = true;
							if (nTestZ == 0)	// 바닥인지 체크
								isFloor = true;

							if (fDistanceToWall > fDepth / 2.0f)
								continue;

							//--- 벽의 경계(모서리)를 표시하기 위한 계산 ---//
							// 타일의 각 모서리에서 플레이어에게 역으로 ray-casting을 한다.
							// 이 광선과 렌더링 광선이 일치할수록 모서리에 가까워지고, 음영을 준다.
							float p[8][2]; // Array to store pairs of (d, dot)

							// 각 모서리에서 플레이어에게 쏜 광선과 렌더링 광선을 내적한 값을 저장
							// (tx, ty, tz) : (0, 0, 0), (0, 0, 1), (0, 1, 0), (0, 1, 1), (1, 0, 0), (1, 0, 1), (1, 1, 0), (1, 1, 1)
							for (int tx = 0; tx < 2; tx++)
							{
								for (int ty = 0; ty < 2; ty++)
								{
									for (int tz = 0; tz < 2; tz++)
									{
										float vx = (float)nTestX + tx - fPlayerX;
										float vy = (float)nTestY + ty - fPlayerY;
										float vz = (float)nTestZ + tz - fPlayerZ;
										float d = sqrt((double)vx * vx + vy * vy + vz * vz);				// 플레이어와 모서리 사이의 거리
										float dot = (fEyeX * vx / d) + (fEyeY * vy / d) + (fEyeZ * vz / d);	// 광선과 모서리에서 쏜 광선 벡터의 내적

										// 배열에 저장
										p[tx * 4 + ty * 2 + tz][0] = d;
										p[tx * 4 + ty * 2 + tz][1] = dot;
									}
								}
							}

							// 거리가 짧은 것부터 긴 순으로 정렬
							for (int i = 0; i < 7; i++) {
								for (int j = 0; j < 7; j++) {
									if (p[j][0] > p[j + 1][0]) {
										float temp = p[j][0];
										p[j][0] = p[j + 1][0];
										p[j + 1][0] = temp;

										float temp1 = p[j][1];
										p[j][1] = p[j + 1][1];
										p[j + 1][1] = temp1;
									}
								}
							}

							// First two/three are closest (we will never see all eight)
							float fBound = 0.01;
							for (int i = 0; i < 7; i++)
							{
								if (acos(p[i][1]) < fBound)
								{
									bBoundary = true;
									break;
								}
							}
						}
					}
				}

				//--- 콘솔 창에 프린트할 nShade 정하기 ---//
				// Shader walls based on distance
				short nShade = ' ';
				if (!isFloor)
				{
					if (fDistanceToWall <= fDepth / 4.0f)		nShade = 0x2588;	// Very close	
					else if (fDistanceToWall < fDepth / 3.0f)	nShade = 0x2593;
					else if (fDistanceToWall < fDepth / 2.0f)	nShade = 0x2592;
					else if (fDistanceToWall < fDepth)			nShade = 0x2591;
					else										nShade = ' ';		// Too far away

					if (bBoundary)		nShade = ' '; // Black it out
				}
				else
				{
					if (fDistanceToWall <= fDepth / 4.0f)		nShade = '#';		// Very close	
					else if (fDistanceToWall < fDepth / 3.0f)	nShade = 'x';
					else if (fDistanceToWall < fDepth / 2.0f)	nShade = '-';
					else if (fDistanceToWall < fDepth)			nShade = '.';
					else										nShade = ' ';		// Too far away
				}
				screen[nowY * nScreenWidth + nowX] = nShade;
			}
		}

		// Display Stats
		swprintf_s(screen, 30, L"X=%3.2f, Y=%3.2f, A=%3.2f ", fPlayerX, fPlayerY, fPlayerA);

		// Display Map
		// Map 초기화
		for (int nx = 0; nx < nMapX; nx++)
			for (int ny = 0; ny < nMapY; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = ' ';
			}
		// 아래에서부터 맵 그리기(바닥 제외)
		for (int nz = nMapZ - 2; nz >= 0; nz--)
		{
			for (int nx = 0; nx < nMapX; nx++)
			{
				for (int ny = 0; ny < nMapY; ny++)
				{
					if (maps[nz][ny * nMapX + nx] == '#')
					{
						if (nx == 0 || ny == 0 || nx == nMapX - 1 || ny == nMapY - 1)
							screen[(ny + 1) * nScreenWidth + nx] = '#';
						else
							screen[(ny + 1) * nScreenWidth + nx] = nz + '0';
					}
				}
			}
		}
		screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'P';

		// Display frame
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, dwWriteCoord, &dwBytesWritten);
		/*
		* 마지막에 Null문자 넣음
		* WriteConsoleOutputCharacter(콘솔 화면 버퍼 핸들, 출력할 문자 배열, 출력할 문자 수,
		* 문자를 출력할 시작 위치를 정하는 COORD 구조체, 실제로 쓰인 문자 수)
		* COORD 구조체: 콘솔 화면 버퍼에서 문자 셀의 좌표를 정의. {0, 0}은 버퍼의 왼쪽 위.
		*/
	}

	free(screen);
	return 0;
}