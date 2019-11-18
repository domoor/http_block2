**http_block2**
-------------

<br>

다른 언어 지원: [한국어](README.md), [English](README.en.md)

### 사용법

> 관리자 모드로 실행<br><br>
> <b>Note:</b> "http_block2.7z" 파일에는 블랙 리스트 URL과 해시화 후 정렬된 블랙 리스트 URL이 포함되어 있다.
> 사용하기 전에 압축 파일을 압축 해제하고 사용해야 한다.



### 설정 사항

> __[ 테스트 환경 ]__  
> Visual Studio 2017 - Debug / x64
>
> __C/C++ - 일반__  
> 추가 포함 디렉터리 : $(SolutionDir)include
>
> __링커 - 일반__  
> 추가 라이브러리 디렉터리 : $(SolutionDir)library
>
> __링커 - 입력__  
> 추가 종속성 : WinDivert.lib;Ws2_32.lib
