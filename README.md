# Runtime Impact Dependent Fracture Simulation in Unreal Engine

## 2022-2 경희소융 펄어비스 인재양성 프로젝트

## 개요
* 경희소융 펄어비스 인재양성 프로젝트 결과물
* Breaking Good : Fracture Modes for Realtime Destruction 논문을 참고하여 언리얼 엔진에 실시간 충격 기반 파괴 시뮬레이션을 구현
* 소개 영상 : https://youtu.be/mJ8dB0lkMps
* 후기 : https://velog.io/@gb_leem/펄어비스-인재양성-프로젝트-후기

## 프로젝트 설명
* 개발 툴 및 사용 언어
  * Unreal Engine 5.0 (blueprint)
  * Python
  * C++
* 개발 방법
  * Python
    * Breaking Good : Fracture Modes for Realtime Destruction 논문 코드를 이용하여 데이터를 얻은 후 Python과 UE간의 tcp 통신을 사용
    * 논문 코드의 파라메터 수정 (num_modes, verbose) 및 함수 return 방식 수정
  * UE
    * Procedural mesh를 사용하여 구현 (https://docs.unrealengine.com/4.27/en-US/BlueprintAPI/Components/ProceduralMesh/)
    * Procedural mesh 생성을 위해 필요한 vertex array, face array, normal, texture 등을 Python에서 받은 string 데이터를 각 데이터 타입으로 가공하여 이용

## 참고 자료
* 논문 프로젝트 사이트 : https://github.com/sgsellan/fracture-modes
* 언리얼 tcp 통신 플러그인 : https://github.com/CodeSpartan/UE4TcpSocketPlugin
