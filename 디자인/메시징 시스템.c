내가 만들 이벤트 시스템의 면모는?

키보드, 마우스 인풋을 처리할 수 있다.
SDL_TimerEvent를 처리할 수 있다.
FPS를 타이머이벤트로 구현할 수 있다.->그런데, 게임루프는? 렌더링과 상태변화는??? 다시 연구하라.

//이런 이벤트들은 SDL_Event와 Event Queue에 대한 것을 이용해야한다.
하지만 내가 만든 객체에서 내가만든 객체로 보내는 이벤트에까지 이것을 쓰고 싶지 않다!

표시객체가 이벤트를 받는다면 모든 자식들에게 전파되어야 할 것이다(물론 안 그럴수도 있음)
발신객체 -> 커스텀메시지 -> 수신객체  //이렇게 되면 좋겠다.
// ㄴ근데 이게 도대체 뭐냐?? 

/*
예시
버튼이 눌려진다. 
버튼 -> 버튼_눌려짐 -> 표시객체: x좌표증가
혹은..
버튼 -> 버튼_눌려짐 -> (눌려진)버튼: Root의 표시객체의 x좌표 증가...
*/


Use Case
버튼을 누르면 값이 출력된다.
다른 버튼을 누르면 다른 값이 출력된다.

SDL_MouseEvent를 얻는다.
SDL_MouseEvent의 좌표값이 버튼이 아니면 아무일도 없어
SDL_MouseEvent의 좌표값이 버튼1이면 "1" 출력 버튼2면 "2"출력...

구현예1
Root는 버튼1, 버튼2, 스프라이트 하나를 가진다. 
입력시스템에서 SDL_Event를 poll한다.
거기서 SDL_MouseEvent를 받으면 그런 일(좌표는 뭐시기고...)이 있었다고 알린다(? 옵저버 패턴 공부하자). - 옵저버패턴의 프로젝트라 하자
Root는 입력시스템을 청취하는 옵저버이다. 
프로젝트의 알림을 자식에게도 전달한다 <- ?
ㄴ메시지 객체는 정보를 가진다. 이 메시지가 재귀적으로(혹은 임시리스트에서) 자식들에게 전달(자식들이 함수를 호출할까?)되겠지..

한편 Root에서 전파되어온 이벤트(메시지?)를 들은 버튼1과 버튼2는 반응을 해야하는데, 
이것을 이벤트리스너를 등록함으로써 구현하자.	<- 그게 뭔데? 어떤 변수가 true면 반응하고, false면 반응하지 않는가? 그런 목록을 가져야하는가?
이벤트리스너는 동적으로 해제될 수 있다.		<- 이번엔 어떤 변수가 false가 된건가? 
ㄴ이것들을 해쉬테이블로 구현하는 건 어떤가? 그렇다면 이벤트리스너 등록은 해쉬테이블<??,??>에 쌍을 넣는 것이고, 해제는 빼는 것이다.
ㄴ모든 이벤트타잎에 관한 enum을 제공하는것도 괜찮을 듯.

어떤 이벤트에 대한 이벤트리스닝을 하는 객체는 이벤트를 들으면 그에 따라 반응한다...

어렵다..

	
	
------------
Use Case
<- 버튼을 누르면 표시객체가 왼쪽으로 움직인다.
-> 버튼을 누르면 표시객체가 오른쪽으로 움직인다.

SDL_MouseEvent를 얻는다.
SDL_MouseEvent의 좌표값이 버튼이 아니면 아무일도 없어