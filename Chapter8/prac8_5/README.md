### [러닝! opencv3] 의 8장 연습문제(p216)  
5번 문제 (mac 환경에서 작업을 했기에 익숙치 않아서 테스트 못한 부분 있음)
~~~
자신만의 간단한 페인트 프로그램을 만든다.
a.이미지를 만들고 0 값으로 설정한 다음 화면에 표시하는 프로그램을 작성한다.
마우스 왼쪽 버튼을 사용해 이미지에 선과 원, 타원, 다각형을 그릴 수 있게 한다.
마우스 오른쪽 버튼을 누르고 있으면 지우개로 동작하는 기능을 만든다.
~~~

```
build 폴더에서 실행파일 실행
$ ./prac8_5

* mouse left button down + drag = 움직이는대로 직선
* ctrl + left button down + drag + button up = 원형
* shift + left button down + drag + button up = 타원형 (원하는 방향대로 그려지지 않음)
* alt + left button down + drag + button up = 직사각형 (mac으로 작업해서 테스트 못해봄)
* right button down + drag = 지우개 모드 (XQuartz 기능에 묻혀서 테스트 못해봄)
```

<img width="499" alt="prac8_5" src="https://github.com/silver7i/Running-OpenCV3/assets/77370836/ce49f633-d3bd-4a1e-b8b3-033a0e8feca6">
