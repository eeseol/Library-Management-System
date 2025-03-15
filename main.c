//2021134043 최이설

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct inventory {
	char last_name[20];
	char first_name[20];
	struct book* head;
}inventory;
typedef struct inventory* p_inventory;

typedef struct book {
	char book_name[50];
	int invent;
	float price;
	struct book* next;
}book;
typedef struct book* p_book;


void greeting();											//환영인사를 출력하는 함수
void readDatabase(inventory theinventory[]);				//입력파일을 읽고 배열을 만드는 함수
void capital(char name[]);									//맨앞 글자는 대문자, 뒷 글자는 소문자로 바꿔주는 함수
int find_theinventory_index(inventory theinventory[], char lastname[], char firstname[]);			//inventory 배열에서 저자가 저장된 index를 반환하는 함수
void input_theinventory(inventory theinventory[], int index, char lastname[], char firstname[]);	//inventory 배열의 index에 저자를 저장하는 함수
void input_node(p_book* newnode, char book_name[], int invent, float price);						//책정보를 구조체에 저장하는 함수
void link_linked(inventory theinventory[], p_book* newnode, int index);								//책정보가 저장된 구조체로 연결리스트를 만드는 함수
char printMenu();											//메뉴정보를 출력하고 선택한 메뉴 정보를 반환하는 함수
void displayInventory(inventory theinventory[]);			//inventory배열의 저자와 저자의 책정보를 출력하는 함수
void displayAuthorsWork(inventory theinventory[]);			//특정 저자명을 입력받고, 그 저자의 책정보를 출력하는 함수 
void totalQty(inventory theinventory[]);					//프로그램에 저장된 전체 책의 개수를 출력하는 함수
void calculateTotalAmount(inventory theinventory[]);		//프로그램에 저장된 책*가격을 출력하는 함수
int addBook(inventory theinventory[]);						//저자명, 책정보를 입력받고 inventory 배열에 추가하는 함수
int changePrice(inventory theinventory[]);					//저자명, 책제목을 입력받고 그 책의 가격을 바꿔주는 함수
int changeQty(inventory theinventory[]);						//저자명, 책제목을 입력받고 그 책의 재고량을 바꿔주는 함수

int main()
{
	inventory theinventory[50] = { '\0', };	//저자명과 책정보를 저장할 inventory 배열 선언
	int flag;								//
	char choice;

	greeting();
	readDatabase(theinventory);

	flag = 1;
	while (flag)
	{
		choice =  printMenu();
		if (choice == '1')
		{
			displayInventory(theinventory);			//inventory배열의 저자와 저자의 책정보를 출력하는 함수
		}
		else if (choice == '2')
		{
			displayAuthorsWork(theinventory);		//특정 저자명을 입력받고, 그 저자의 책정보를 출력하는 함수
		}
		else if (choice == '3')
		{
			addBook(theinventory);					//저자명, 책정보를 입력받고 inventory 배열에 추가하는 함수
		}
		else if (choice == '4')
		{
			changePrice(theinventory);				//저자명, 책제목을 입력받고 그 책의 가격을 바꿔주는 함수
		}
		else if (choice == '5')
		{
			changeQty(theinventory);					//저자명, 책제목을 입력받고 그 책의 재고량을 바꿔주는 함수
		}
		else if (choice == '6')
		{
			totalQty(theinventory);					//프로그램에 저장된 전체 책의 개수를 출력하는 함수
		}
		else if (choice == '7')
		{
			calculateTotalAmount(theinventory);		//프로그램에 저장된 책*가격을 출력하는 함수
		}
		else if (choice == '8')
		{
			printf("\n프로그램을 이용해 주셔서 감사합니다.\n");
			printf("프로그램이 종료되었습니다.\n");
			flag = 0;
		}
		else
		{
			printf("Invalid choice\n");
		}
	}
	
	return 0;
}

/* 환영인사 출력하는 함수 */
void greeting()
{
	printf("서점 재고 관리 프로그램에 오신 것을 환영합니다.\n\n");
}

/*입력파일을 읽고 배열을 만드는 함수  */
void readDatabase(inventory theinventory[])
{
	int i = 0, index = 0;				//i = 입력파일 배열의 index, index = theinventory배열의 index
	char file_name[30] = { '\0', };		//입력파일의 위치를 저장하는 배열 선언
	FILE* inptr;						

	printf("데이터가 저장된 파일 위치를 입력해 주세요 : ");
	scanf("%s", file_name);					//파일위치를 입력받음
	getchar();

	inptr = fopen(file_name, "r");

	if (inptr == NULL)
	{
		printf("%s에 있는 파일을 열 수 없습니다.\n", file_name);
		exit(1);
	}

	while (!feof(inptr))							/* 파일의 모든데이터를 입력받을때까지 반복문 */
	{
		char read[100];																						/* 변수 선언 */
		char* parr = fgets(read, 100, inptr);												//parr = 파일에서 한줄씩 데이터를 읽어 저장하는 변수
		char lastname[20] = { '\0', }, firstname[20] = { '\0', }, bookname[50] = { '\0', };	//새로 저장된 저자명, 책제목을 저장하는 변수들
		int newinvent = 0;																	//newinvent = 새로 저장된 재고량을 저장하는 변수
		float newprice = 0;																	//newprice = 새로 저장된 가격을 저장하는 변수
		p_book newnode = NULL;																//newnode = 책정보를 가리키는 포인터 선언
		int m = 0, n = 0, real_num = 1;														//m=n= 정보를 저장할때 index로 사용 / real_num = 가격저장시 (소수점 저장)사용

		if (parr == NULL)			//읽은 데이터가 없을때
		{
			break;							//반복문 탈출
		}

		while (read[m] != ',')		//lastname을 저장하는 반복문
		{
			lastname[n] = read[m];			//읽은 데이터를 lastname에 복사
			m++, n++;
		}
		lastname[m] = '\0';					//lastname끝에 널문자 저장
		n = 0, m++;
		
		while (read[m] != ',')		//firstname을 저장하는 반복문
		{

			firstname[n] = read[m];			//읽은 데이터를 lastname에 복사
			m++, n++;
		}
		firstname[n] = '\0';				//firstname끝에 널문자 저장
		n = 0, m++;

		while (read[m] != ',')		//bookname을 저장하는 반복문
		{
			bookname[n] = read[m];			//읽은 데이터를 bookname에 복사
			m++, n++;
		}
		bookname[n] = '\0';					//bookname끝에 널문자 저장
		m++;

		while (read[m] != ',')		//재고를 저장하는 반복문
		{
			newinvent = (newinvent * 10) + ((int)read[m] - 48);	//읽은데이터(문자형)를 int형으로 변환해 저장함
			m++;
		}
		m++;

		while (read[m] != '.')		//가격을 저장하는 반복문(정수부)
		{
			newprice = (newprice * 10) + ((int)read[m] - 48);	//가격의 정수부 -> 읽은데이터(문자형)를 float형으로 변환해 저장함
			m++;
		}
		m++;

		while ((read[m] != '\n') && (read[m] != '\0'))	//가격을 저장하는 반복문(실수부)
		{
			newprice = newprice + ((int)read[m] - 48) * (float)pow(10, -real_num);	//가격의 실수부 -> 읽은데이터(문자형)를 float형으로 변환해 저장함
			m++, real_num++;
		}

		capital(lastname);			//lastname의 대소문자 정리
		capital(firstname);			//firstname의 대소문자 정리

		index = find_theinventory_index(theinventory, lastname, firstname);	//inventory 배열에서 저자가 저장된 index를 반환하는 함수 ghcnf

		if (index == -2000)			//index값을 찾을때 오류가 생긴 경우
		{
			printf("theinventory 저장 오류.\n 관리자에게 문의해 주세요\n");
			exit(1);
		}
		
		if (index >= 0)				//새로운 저자가 입력됐을경우
		{
			input_theinventory(theinventory, index, lastname, firstname);	//inventory배열에 저자를 저장하는 함수 호출
		}
		

		input_node(&newnode, bookname, newinvent, newprice);				//책정보를 구조체에 넣는 함수 호출
		link_linked(theinventory, &newnode, index);							//책정보가 있는 구조체를 연결리스트로 만드는 함수 호출

	}
	fclose(inptr);
}

/* 맨앞 글자는 대문자, 뒷 글자는 소문자로 바꿔주는 함수 */
void capital(char name[])
{
	int index = 0;

	while (name[index] != '\0')		//배열의 데이터가 null이 아닐때까지 반복문
	{
		if (index == 0)				//첫 문자일때
		{
			if ((97 <= name[index]) && (name[index] <= 122))
				name[index] = name[index] - 32;					//대문자로 바꿔줌

		}
		else						//첫 문자가 아닐때
		{
			if ((65 <= name[index]) && (name[index] <= 90))
				name[index] = name[index] + 32;					//소문자로 바꿔줌
		}
		index++;
	}
}

/* inventory 배열에서 저자가 저장된 index를 반환하는 함수 */
int find_theinventory_index(inventory theinventory[], char lastname[], char firstname[])
{
	int index = 1, re = 0;			//index = theinventory의 index / re = index값을 임시저장하는 변수
	int i = 0, rest = 0, cnt = 0;	//i = 저자명 비교시 index로 사용 / rest = ','를 count하는 변수 / cnt = 오류로 인해 반복문이 무한으로 돌아가는 것을 방지하는 변수

	if (theinventory[index].first_name[0] == '\0')	//첫 저장일때
	{
		return index;
	}
	while (1)										//첫 저장이 아닐때
	{
		while ((lastname[i] != '\0') && (lastname[i] != '\n'))		//lastname에 데이터가 없을때까지 반복
		{
			if ((theinventory[index].last_name[i] == 0) || (theinventory[index].last_name[i] > lastname[i]))	//theinventory[index]에 저장된 저자보다 앞에 저장돼야 할때
			{
				return index;		
			}
			else if (theinventory[index].last_name[i] < lastname[i])		//theinventory[index]에 저장된 저자보다 뒤에 저장돼야 할때
			{
				index++;															
				i = 0;
				continue;															//index 추가후 다시 비교하기
			}
			else if (theinventory[index].last_name[i] == lastname[i])		//theinventory[index]에 저장된 저자와 같을때
			{
				i++;																//다음 글자 다시 비교 (데이터가 없을시 while문의 조건식에 의해 탈출)
			}
		}
		i = 0;

		while ((firstname[i] != '\0') && (firstname[i] != '\n'))	//firstname에 데이터가 없을때까지 반복
		{
			if  (theinventory[index].first_name[i] > firstname[i])			//theinventory[index]에 저장된 저자보다 앞에 저장돼야 할때
			{
				return index;
			}
			else if (theinventory[index].first_name[i] < firstname[i])		//theinventory[index]에 저장된 저자보다 뒤에 저장돼야 할때
			{
				index++;
				i = 0;
				break;																//index추가후 lastname부터 다시 비교
			}
			else															//theinventory[index]에 저장된 저자와 같을때
			{
				if ((theinventory[index].first_name[i] == 0))						//모든 이름이 같을때
				{
					return -index;													//중복이 있으므로 -index 반환
				}
			}
			i++;
		}

		if (theinventory[index].first_name[i] == 0)		//모든 이름이 같을때
			return -index;										//중복이 있으므로 -index 반환

		cnt++;
		if (cnt > 50)				//오류로 인해 반복문이 무한으로 돌아갈때
		{
			printf("find_theinventory_index 오류 (cnt)\n 관리자에게 문의해 주세요.\n");
			exit(1);								//프로그램 종료
		}
	}
	return -2000;		//오류로 인해 반복문 내에서 함수값이 반환되지 않았을때 -2000 반환
}

/* inventory 배열의 index에 저자를 저장하는 함수 */
void input_theinventory(inventory theinventory[], int index, char lastname[], char firstname[])
{
	inventory temp1, temp2;			//temp1 = temp2 = theinventory에서 한 칸씩 옮겨야될때 사용하는 변수
	int re = index, j = 0, m = 0;	//re = index값을 임시저장하는 변수 / j = m 저자명의 index에 사용되는 변수

	if (index>0)		//초기 저장일 경우 (한칸씩 밀어줘야 될 가능성이 있음)
	{
		temp1 = theinventory[re++];
		while (theinventory[re - 1].first_name[j] != '\0')	//theinventory 배열을 한칸씩 밀어주는 반복문
		{
			temp2 = theinventory[re];
			theinventory[re++] = temp1;

			temp1 = theinventory[re];
			theinventory[re++] = temp2;
		}
			theinventory[index].head = NULL;
	}
	while (lastname[m] != '\0')		//lastname에 데이터가 없을때까지 반복
	{
		theinventory[index].last_name[m] = lastname[m];	//theinventory의 lastname에 저장
		m++;
	}
	theinventory[index].last_name[m] = '\0';			//theinventory의 lastname 마지막에 널문자 저장
	m = 0;

	while (firstname[m] != '\0')	//firstname에 데이터가 없을때까지 반복
	{
		theinventory[index].first_name[m] = firstname[m];//theinventory의 firstname에 저장
		m++;
	}
	theinventory[index].first_name[m] = '\0';			//theinventory의 firstname 마지막에 널문자 저장
	
}

/* 책정보를 구조체에 저장하는 함수 */
void input_node(p_book* newnode, char book_name[], int newinvent, float newprice)
{
	int i = 0;									//i = 책제목의 index에 사용되는 변수
	*newnode = (p_book)malloc(sizeof(book));	//책정보를 저장할 공간 할당

	if (*newnode == NULL)			//공간할당에 오류가 생겼을 경우
	{
		printf("연결리스트 생성에 문제가 생겼습니다.(input_node)\n관리자에게 문의해 주세요.\n");
		exit(1);
	}
	while (book_name[i] != '\0')	//bookname에 데이터가 없을때까지 반복
	{
		(*newnode)->book_name[i] = book_name[i];//구조체에 bookname 저장
		i++;
	}
	(*newnode)->book_name[i] = '\0';			//구조체 bookname 끝에 널문자 저장
	(*newnode)->invent = newinvent;				//구조체에 재고량 저장
	(*newnode)->price = newprice;				//구조체에 가격 저장
	(*newnode)->next = NULL;					//구조체 next(다음노드 가리킴)에 널문자 저장
	
}

/* 책정보가 저장된 구조체로 연결리스트를 만드는 함수 */
void link_linked(inventory theinventory[], p_book* newnode, int index)
{
	int i = 0;			//i = 책제목의 index에 사용되는 변수
	int answer = 0;		//책제목이 중복됐을 경우 사용자의 의사를 저장하는 변수
	int cnt = 0;		//오류로 인해 반복문이 무한으로 돌아가는 것을 막는 변수

	if (index < 0)		//이미 저자의 책이 한권이상 저장되어있을 경우
	{
		p_book temp = NULL;
		p_book preptr = NULL;
		index = -index;		//초기 저장이 아닌경우 index값 반환할때 음수로 반환. 양수로 바꿔줌
		temp = theinventory[index].head;

		while (1)
		{
			if (temp->book_name[i] < (*newnode)->book_name[i])		//기존 책보다 뒤에 들어가야 될때
			{
;
				if (temp->next != '\0')										//뒤에 책이 있을때
				{
					preptr = temp;
					temp = temp->next;												//다음 책으로 이동후 다시 비교
					i = 0;
				}
				else														//뒤에 책이 없을때
				{
					temp->next = *newnode;											//기존 책 뒤에 저장
					break;
				}
			}
			else if (temp->book_name[i] > (*newnode)->book_name[i])	//기존 책보다 앞에 들어가야 될때
			{
				if (preptr == NULL)											//앞에 책이 없을때
				{
					(*newnode)->next = temp;
					theinventory[index].head = (*newnode);							//head로 저장
					break;
				}
				else														//앞에 책이 있을때
				{
					preptr->next = (*newnode);
					(*newnode)->next = temp;										
					break;
				}
			}
			else if (temp->book_name[i] == (*newnode)->book_name[i])//기존 책과 책이름이 같을때
			{
				if (temp->book_name[i] == 0)							//기존 책과 책이름이 완전히 같을때
				{
					printf("\n입력한 저자의 입력한 책이 이미 있습니다.\n");
					printf("기존 재고량 : %d\n", temp->invent);			//재고량을 수정할건지 의사를 물어봄
					printf("입력 재고량 : %d\n", (*newnode)->invent);
					printf("기존 재고량을 유지하려면 1, 입력 재고량을 저장하려면 2, 기존재고량 + 입력재고량을 저장하려면 3을 입력해주세요. : \n");
					scanf("%d", &answer);

					while ((1 > answer) || (answer > 3))				//사용자가 선택지 외의 입력을 했을경우
					{
						printf("입력 오류입니다.\n");
						printf("기존 재고량을 유지하려면 1, 입력 재고량을 저장하려면 2, 기존재고량 + 입력재고량을 저장하려면 3을 입력해주세요. : \n");
						scanf("%d", &answer);
					}
					if (answer == 1)
					{
						(*newnode)->invent = temp->invent;
					}
					else if (answer == 2)		//입력한 재고량을 저장할 경우
					{
						temp->invent = (*newnode)->invent;
					}
					else if (answer == 3)	// 기존재고량 + 입력재고량을 저장하는 경우
					{
						temp->invent = temp->invent + (*newnode)->invent;
						(*newnode)->invent = temp->invent;
					}

					printf("\n기존 가격 : %.2f\n", temp->price);		//가격정보를 수정할건지 의사를 물어봄
					printf("입력 가격 : %.2f\n", (*newnode)->price);
					printf("기존 가격을 유지하려면 1, 입력 가격으로 업데이트 하려면 2를 입력해 주세요. : ");
					scanf("%d", &answer);

					while ((1 > answer) || (answer > 2))				//사용자가 선택지 외의 입력을 했을경우
					{
						printf("입력 오류입니다.\n");
						printf("기존 가격을 유지하려면 1, 입력 가격으로 업데이트 하려면 2를 입력해 주세요. : ");
						scanf("%d", &answer);
					}
					if (answer == 1)
					{
						(*newnode)->price = temp->price;
					}
					else if (answer == 2)			//입력 가격으로 업데이트 할 경우
					{
						temp->price = (*newnode)->price;
					}
					printf("성공적으로 저장되었습니다.\n");
					break;
				}
				i++;
			}
			cnt++;
			if (cnt > 50)				//오류로 인해 반복문이 무한으로 돌아갈때
			{
				printf("link_linked 오류 (cnt)\n 관리자에게 문의해 주세요.\n");
				exit(1);								//프로그램 종료
			}
		}
	}
	else		//책 저장이 처음일때
	{
		theinventory[index].head = *newnode;	//head로 저장
		theinventory[index].head->next = NULL;
	}
}

/* 메뉴정보를 출력하고 선택한 메뉴 정보를 반환하는 함수 */
char printMenu()
{
	char choice[50] = { '\0', };	//사용자가 어떤 메뉴를 선택하는지 저장하는 변수

	printf("------------------------------------------------\n");
	printf("Enter 1 to display the inventory\n");
	printf("Enter 2 to display the books by one author\n");
	printf("Enter 3 to add a book\n");
	printf("Enter 4 to change the price\n");
	printf("Enter 5 to change the qty on hand\n");
	printf("Enter 6 to view the total number of books in the inventory\n");
	printf("Enter 7 to see the total amount of the entire inventory\n");
	printf("Enter 8 to exit\n");
	printf("\n원하는 메뉴를 입력해 주세요 : ");

	scanf("%s", choice);

	while (('1' > choice[0]) || (choice[0] > '8'))				//사용자가 선택지 외의 입력을 했을경우
	{
		getchar();
		printf("입력 오류\n");
		printf("\n메뉴(숫자)를 정확하게 입력해 주세요 : ");
		scanf("%s", choice);
		getchar();
	}
	return choice[0];
}

/* inventory배열의 저자와 저자의 책정보를 출력하는 함수 */
void displayInventory(inventory theinventory[])
{
	int index = 1;	//theinventory의 index값으로 사용되는 변수
	printf("%8c---\n", '-');
	while (theinventory[index].last_name[0] != '\0')	//theinventory에 데이터가 없을때까지 반복
	{
		printf("The author is : %s, %s\n", theinventory[index].last_name, theinventory[index].first_name);
		p_book read_temp = theinventory[index].head;
		while (read_temp != NULL)
		{
			printf("%8c The title is: %s\n", ' ', read_temp->book_name);
			printf("%8c The qty is : %d\n", ' ', read_temp->invent);
			printf("%8c The price is : %.2f\n\n", ' ', read_temp->price);
			printf("%8c---\n", '-');
			read_temp = read_temp->next;
		}
		index++;
	}

}

/* 특정 저자명을 입력받고, 그 저자의 책정보를 출력하는 함수 */
void displayAuthorsWork(inventory theinventory[])
{															/* 변수 선언 */
	char findFirstName[20] = { '\0', };	//findFirstName = 찾고자 하는 저자의 firstname을 저장하는 변수
	char findLastName[20] = { '\0', };	//findLasstName = 찾고자 하는 저자의 lastname을 저장하는 변수
	int index = 0;						//theinventory의 index로 사용되는 변수
	p_book temp;

	//getchar();
	printf("Enter the author's first name : ");
	scanf("%s", findFirstName);
	getchar();
	printf("Enter the author's last name : ");
	scanf("%s", findLastName);
	getchar();


	capital(findFirstName);		//firstname의 대소문자를 정리해주는 함수호출
	capital(findLastName);		//lastname의 대소문자를 정리해주는 함수호출

	index = find_theinventory_index(theinventory, findLastName, findFirstName);	//inventory 배열에서 저자가 저장된 index를 반환하는 함수 호출

	if (index == -2000)			//index값을 찾을때 오류가 생긴 경우
	{
		printf("index찾기 오류(displayAuthorsWork)\n관리자에게 문의해 주세요.\n");
		exit(1);
	}
	else if (index < 0)			//입력한 저자가 theinventory에 저장되어 있는 경우
	{
		index = -index;					// find_theinventory_index 함수에서 입력한 저자가 theinventory에 저장되어 있으면 -index값을 반환하므로 
		temp = theinventory[index].head;

		printf("\n");

		while (temp != NULL)			//temp에 저장될 책정보가 없을때까지 반복
		{
			printf("%8c The title is: %s\n", ' ',temp->book_name);
			printf("%8c The qty is : %d\n", ' ', temp->invent);
			printf("%8c The price is : %.2f\n\n", ' ', temp->price);
			printf("%8c---\n", '-');
			temp = temp->next;
		}
	}
	else						//입력한 저자가 theinventory에 저장되어 있지 않은 경우
	{
		printf("저장된 저자명 중 %s, %s을 찾을 수 없습니다.\n", findLastName, findFirstName);
	}

}

/* 프로그램에 저장된 전체 책의 개수를 출력하는 함수 */
void totalQty(inventory theinventory[])
{
	int invent = 0;		//재고량을 저장하는 변수
	int k = 1;			//theinventory의 index로 사용되는 변수

	while (theinventory[k].last_name[0] != '\0')	//저자가 없을때까지 반복
	{
		p_book read_temp = theinventory[k].head;
		while (read_temp != NULL)					//책정보가 없을때까지 반복
		{
			invent = invent + read_temp->invent;		//재고량 저장
			read_temp = read_temp->next;
		}
		k++;
	}

	printf("\n저장된 책은 모두 [ %d ] 권입니다.\n\n", invent);

}

/* 프로그램에 저장된 책*가격을 출력하는 함수 */
void calculateTotalAmount(inventory theinventory[])
{
	float price = 0;	//가격을 저장하는 변수
	int index = 1;		//theinventory의 index로 사용되는 변수

	while (theinventory[index].last_name[0] != '\0')		//저자가 없을때까지 반복
	{
		p_book read_temp = theinventory[index].head;
		while (read_temp != NULL)							//책정보가 없을때까지 반복
		{
			price = price + read_temp->invent * read_temp->price;	//가격 저장
			read_temp = read_temp->next;
		}
		index++;
	}

	printf("\n저장된 책들의 총 가격은 [ %.2f ] 입니다. \n\n", price);

}

/* 저자명, 책정보를 입력받고 theinventory 배열에 추가하는 함수 */
int addBook(inventory theinventory[])
{																/* 변수 선언 */
	char EnterFirstName[20] = { '\0', };	//EnterFirstName = 찾고자 하는 저자의 firstname을 저장하는 변수
	char EnterLastName[20] = { '\0', };		//EnterLastName = 찾고자 하는 저자의 lastname을 저장하는 변수
	char EnterBookName[50] = { '\0', };		//EnterBookName = 찾고자 하는 저자의 bookname을 저장하는 변수
	int index = 0, answer = 0;				//theinventory의 index로 사용되는 변수 / answer = 사용자의 의사를 저장하는 변수
	int invent = 0;							//재고량을 저장하는 변수
	float price = 0, enterInvent = 0;		//price = 가격을 저장하는 변수 / enterInvent = 재고량이 제대로 입력됐는지 확인하기 위한 변수
	p_book newnode;

	printf("Enter the author's first name : ");	//저자명 입력받음
	scanf("%s", EnterFirstName);
	getchar();
	printf("Enter the author's last name : ");
	scanf("%s", EnterLastName);
	getchar();
	
	capital(EnterFirstName);					//firstname의 대소문자를 정리해주는 함수 호출
	capital(EnterLastName);						//lastname의 대소문자를 정리해주는 함수 호출

	printf("Enter the title : ");				//책제목 입력받음
	gets_s(EnterBookName,sizeof(EnterBookName));

	printf("Enter the qty : ");
	scanf("%f", &enterInvent);
	invent = (int)enterInvent;

	while ((invent != enterInvent) || (invent <= 0))		//입력받은 재고량값이 유효하지 않을 경우 다시 입력받음
	{
		printf("재고량은 양의 정수여야 합니다.\n");
		printf("재고량을 정확히 입력해 주세요.\n");
		printf("changePrice를 취소하고 메뉴로 돌아가시려면 - 1, 다시 입력하려면 양의 정수를 입력해 주세요 : ");
		scanf("%f", &enterInvent);
		invent = (int)enterInvent;
		if (invent == -1)										//사용자가 저장을 원하지 않을때
			return 0;
	}

	printf("Enter the price : ");
	scanf("%f", &price);

	while (price <= 0)			//입력받은 가격이 유효값이 아닌 경우
	{
		printf("가격은 양의 실수여야 합니다.\n");
		printf("가격을 정확히 입력해 주세요.\n");
		printf("changePrice를 취소하고 메뉴로 돌아가시려면 - 1, 다시 입력하려면 양의 실수를 입력해 주세요 : ");
		scanf("%f", &price);
		if (price == -1)					//사용자가 저장을 원하지 않을때
			return 0;
	}

	index = find_theinventory_index(theinventory, EnterLastName, EnterFirstName);		//inventory 배열에서 저자가 저장된 index를 반환하는 함수호출

	if (index == -2000)			//index값을 찾을때 오류가 생긴 경우
	{
		printf("theinventory 저장 오류.(addBook)\n 관리자에게 문의해 주세요\n");
		exit(1);
	}
	if (index >= 0)				//theinventory 배열에 입력된 저자가 없을때
	{
		input_theinventory(theinventory, index, EnterLastName, EnterFirstName);
		input_node(&newnode, EnterBookName, invent, price);
	}
	if (index < 0)				//theinventory 배열에 입력된 저자가 있을때
	{
		printf("\n%s, %s 저자가 이미 저장되어 있습니다.\n", EnterLastName, EnterFirstName);
		printf("메뉴로 돌아가려면 1, 책정보를 저장하려면 2를 입력해 주세요 : ");
		scanf("%d", &answer);
		if (answer == 1)			//사용자가 저장을 원하지 않을때
			return 0;
		input_node(&newnode, EnterBookName, invent, price);
	}
	link_linked(theinventory, &newnode, index);

	printf("\n다음과 같은 정보가 성공적으로 저장되었습니다.\n");
	printf("The author is : %s, %s\n", EnterLastName, EnterFirstName);
	printf("The title is : %s\n", EnterBookName);
	printf("The qty is : %d\n", newnode->invent);
	printf("The price is : %.2f\n", newnode->price);

	return 0;
}

/* 저자명, 책제목을 입력받고 그 책의 가격을 바꿔주는 함수 */
int changePrice(inventory theinventory[])
{																	/* 변수 선언 */
	char EnterFirstName[20] = { '\0', };	//EnterFirstName = 찾고자 하는 저자의 firstname을 저장하는 변수
	char EnterLastName[20] = { '\0', };		//EnterLastName = 찾고자 하는 저자의 lastname을 저장하는 변수
	char EnterBookName[50] = { '\0', };		//EnterBookName = 찾고자 하는 저자의 bookname을 저장하는 변수
	int index = 0, answer = 0;				//theinventory의 index로 사용되는 변수 / answer = 사용자의 의사를 저장하는 변수
	int invent = 0, i = 0;					//재고량을 저장하는 변수
	float price = 0, enterInvent = 0;		//price = 가격을 저장하는 변수 / enterInvent = 재고량이 제대로 입력됐는지 확인하기 위한 변수

	printf("Enter the author's first name : ");		//저자명 입력받음
	scanf("%s", EnterFirstName);
	getchar();
	printf("Enter the author's last name : ");
	scanf("%s", EnterLastName);
	getchar();

	capital(EnterFirstName);			//firstname 대소문자 정리
	capital(EnterLastName);				//lastname 대소문자 정리

	index = find_theinventory_index(theinventory, EnterLastName, EnterFirstName);		//inventory 배열에서 저자가 저장된 index를 반환하는 함수호출

	while (index >= 0)					//저장된 데이터에 입력한 저자명이 없는 경우
	{
		printf("\n\n저장된 데이터 중 입력한 저자명과 같은 데이터가 없습니다.\n");
		printf("저자명을 다시 입력하려면 1, 메뉴로 돌아가려면 2를 입력해 주세요 : ");
		scanf("%d", &answer);
		if (answer == 2)						//사용자가 입력을 원하지 않을때
			return 0;
		printf("Enter the author's first name : ");		//저자명 입력받음
		scanf("%s", EnterFirstName);
		getchar();
		printf("Enter the author's last name : ");
		scanf("%s", EnterLastName);
		getchar();
		capital(EnterFirstName);		//firstname 대소문자 정리
		capital(EnterLastName);			//lastname 대소문자 정리
		index = find_theinventory_index(theinventory, EnterLastName, EnterFirstName);
	}

	printf("Enter the title : ");		//책제목 입력받기
	gets_s(EnterBookName, sizeof(EnterBookName));

	p_book temp = NULL;
	p_book preptr = NULL;
	index = -index;
	temp = theinventory[index].head;

	while (1)
	{
		if (temp->book_name[i] < EnterBookName[i])		//기존 책보다 뒤에 들어가야 될때
		{
			;
			if (temp->next != '\0')	//뒤에 책이 있을때
			{
				preptr = temp;
				temp = temp->next;
				i = 0;
			}
			else					//뒤에 책이 없을때
			{
				printf("\n입력한 책이름과 같은 책이 없습니다.\n\n");
				return 0;
			}
		}
		else if (temp->book_name[i] > EnterBookName[i])	//기존 책보다 앞에 들어가야 될때
		{
			printf("입력한 책이름과 같은 책이 없습니다.\n");
			return 0;
		}
		else if (temp->book_name[i] == EnterBookName[i])//기존 책과 같을때
		{
			if (temp->book_name[i] == 0)				//기존 책과 완전히 같을때
			{
				printf("Enter the price : ");				//가격 입력받기
				scanf("%f", &price);

				while (price <= 0)							//입력한 가격이 유효하지 않을때
				{
					printf("가격은 양의 실수여야 합니다.\n");
					printf("가격을 정확히 입력해 주세요.\n");
					printf("changePrice를 취소하고 메뉴로 돌아가시려면 - 1, 다시 입력하려면 양의 실수를 입력해 주세요 : ");
					scanf("%f", &price);
					if (price == -1)						//사용자가 입력을 원하지 않을때
						return 0;
				}
				temp->price = price;
				break;
			}
			i++;
		}
	}

	printf("\n다음과 같이 가격정보가 업데이트 되었습니다.\n");
	printf("The author is : %s, %s\n", EnterLastName, EnterFirstName);
	printf("The title is : %s\n", EnterBookName);
	printf("The qty is : %d\n", temp->invent);
	printf("The price is : %.2f\n", temp->price);

	return 0;
}

/* 저자명, 책제목을 입력받고 그 책의 재고량을 바꿔주는 함수 */
int changeQty(inventory theinventory[])
{																	/* 변수 선언 */
	char EnterFirstName[20] = { '\0', };	//EnterFirstName = 찾고자 하는 저자의 firstname을 저장하는 변수
	char EnterLastName[20] = { '\0', };		//EnterLastName = 찾고자 하는 저자의 lastname을 저장하는 변수
	char EnterBookName[50] = { '\0', };		//EnterBookName = 찾고자 하는 저자의 bookname을 저장하는 변수
	int index = 0;							// theinventory의 index로 사용되는 변수 
	int invent = 0, i = 0;					//재고량을 저장하는 변수
	float enterInvent = 0;					// enterInvent = 재고량이 제대로 입력됐는지 확인하기 위한 변수
	int answer = 0;							// answer = 사용자의 의사를 저장하는 변수
	
	printf("Enter the author's first name : ");		//저자명 입력받기
	scanf("%s", EnterFirstName);
	getchar();
	printf("Enter the author's last name : ");
	scanf("%s", EnterLastName);
	getchar();

	capital(EnterFirstName);		//firstname 대소문자 정리
	capital(EnterLastName);			//lastname 대소문자 정리

	index = find_theinventory_index(theinventory, EnterLastName, EnterFirstName);		//inventory 배열에서 저자가 저장된 index를 반환하는 함수호출

	while (index >= 0)				//저장된 데이터에 입력한 저자명이 없는 경우
	{
		printf("\n저장된 데이터 중 입력한 저자명과 같은 데이터가 없습니다.\n");
		printf("저자명을 다시 입력하려면 1, 메뉴로 돌아가려면 2를 입력해 주세요 : ");
		scanf("%d", &answer);
		if (answer == 2)				//사용자가 입력을 원하지 않을때
			return 0;
		printf("Enter the author's first name : ");		//저자명 입력받음
		scanf("%s", EnterFirstName);
		getchar();
		printf("Enter the author's last name : ");
		scanf("%s", EnterLastName);
		getchar();
		capital(EnterFirstName);		//firstname 대소문자 정리
		capital(EnterLastName);			//lastname 대소문자 정리
		index = find_theinventory_index(theinventory, EnterLastName, EnterFirstName);
	}

	printf("Enter the title : ");		//책제목 입력받기
	gets_s(EnterBookName, sizeof(EnterBookName));


	p_book temp = NULL;
	p_book preptr = NULL;
	index = -index;
	temp = theinventory[index].head;

	while (1)
	{
		if (temp->book_name[i] < EnterBookName[i]) //기존 책보다 뒤에 들어가야 될때
		{
			;
			if (temp->next != '\0')	//뒤에 책이 있을때
			{
				preptr = temp;
				temp = temp->next;
				i = 0;
			}
			else					//뒤에 책이 없을때
			{
				printf("입력한 책이름과 같은 책이 없습니다.\n");
				return 0;
			}
		}
		else if (temp->book_name[i] > EnterBookName[i])//기존 책보다 앞에 들어가야 될때
		{
			printf("입력한 책이름과 같은 책이 없습니다.\n");
			return 0;
		}
		else if (temp->book_name[i] == EnterBookName[i])//기존 책과 같을때
		{
			if (temp->book_name[i] == 0)					//기존 책과 완전히 같을때
			{
				printf("Enter the qty : ");						//재고량 입력받기
				scanf("%d", &invent);

				while (invent <= 0)								//입력받은 재고량이 유효하지 않을때
				{
					printf("재고는 양의 정수여야 합니다.\n");
					printf("재고를 정확히 입력해 주세요.\n");
					printf("changeQty를 취소하고 메뉴로 돌아가시려면 - 1, 다시 입력하려면 양의 실수를 입력해 주세요 : ");
					scanf("%d", &invent);
					if (invent == -1)							//사용자가 저장을 원하지 않을때
						return 0;
				}
				temp->invent = invent;
				break;
			}
			i++;
		}
	}
	printf("\n다음과 같이 가격정보가 업데이트 되었습니다.\n");
	printf("The author is : %s, %s\n", EnterLastName, EnterFirstName);
	printf("The title is : %s\n", EnterBookName);
	printf("The qty is : %d\n", temp->invent);
	printf("The price is : %.2f\n", temp->price);

	return 0;
}
