# ConsoleRendering

숭실대학교  23년 2학기 프로그래밍및실습2 수업 기말 프로젝트

작업 기간: 11/9 ~ 11/26

[![Code-It-Yourself! First Person Shooter (Quick and Simple C++)](https://img.youtube.com/vi/xW8skO7MFYw/0.jpg)](https://www.youtube.com/watch?v=xW8skO7MFYw)

해당 영상을 응용하여 3차원 물체를 콘솔 창에 렌더링 하는 프로젝트 입니다.

### 참고 영상 분석

![원본프로젝트분석1](https://github.com/9dyy/ConsoleRendering/blob/main/%EA%B8%B0%EC%A1%B4%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8.png)

참고 영상에서는 2차원 map을 ray-casting하여 화면을 생성함.

#### Ray Casting 이란?
![RayCasting](https://github.com/9dyy/ConsoleRendering/blob/main/raycasting.png)

광선을 쏴서 물체 또는 벽을 탐지하는 것

참고 영상에서는 광선이 fStepSize(0.1) 만큼씩 증가하며 전진한다


### 프로젝트 설명
![NewProject](https://github.com/9dyy/ConsoleRendering/blob/main/%EC%83%88%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8.png)

참고 영상의 코드를 참고하여 ray casting과 화면 출력 관련 부분을 구현함

3차원 map을 만들어서 사용하여 ray casting해서 화면을 생성하도록 변경

이번 프로젝트는 3차원 물체의 ray casting이 주 목적이므로, 3차원 map은 2차원 map을 여러 겹 쌓아 간단하게 만듦

화면 왼쪽 상단부터 ray-casting을 하는데, 광선이 투영되는 각도가 필요함. 따라서 해당 각도를 다음과 같이 구한다.

![참고영상구현부](https://github.com/9dyy/ConsoleRendering/blob/main/fRayAngle_XY.png)

![프로젝트추가한부분](https://github.com/9dyy/ConsoleRendering/blob/main/fRayAngle_Z%20.png)

광선이 투영되는 각도를 구했으면, 해당 각도의 단위벡터당 X, Y, Z의 증가량을 구한다.

해당 값으로 광선이 플레이어로부터 발사돼서 움직일 때마다  X, Y, Z가 어느 좌표에 위치하는지 구한다.

![unitVector구하기](https://github.com/9dyy/ConsoleRendering/blob/main/unitvector.png)

### 프로젝트 결과
![정육면체](https://github.com/9dyy/ConsoleRendering/blob/main/%EC%A0%95%EC%9C%A1%EB%A9%B4%EC%B2%B4.png)
![정사각뿔](https://github.com/9dyy/ConsoleRendering/blob/main/%EC%A0%95%EC%82%AC%EA%B0%81%EB%BF%94.png)
![역정사각뿔](https://github.com/9dyy/ConsoleRendering/blob/main/%EC%97%AD%EC%A0%95%EC%82%AC%EA%B0%81%EB%BF%94.png)



