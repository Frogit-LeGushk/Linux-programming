#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAX_STACK_SIZE (1 << 10)
#define EMPTY "empty"
#define MAX_OBJECTS (1 << 4)
#define MAX_LIST_SIZE (MAX_STACK_SIZE * MAX_OBJECTS)
const bool ALERT_LOGS = true;


// 1) create existing data types in virtual mashine
typedef enum {
    NUMBER,
    STRING,
    TWIN
} ObjectType;


// 2) discribe container of every data type
typedef struct SObject {
    ObjectType type;
    bool isMarked;

    struct SObject* next; // <-- pointer for List of all objects

    union {
        double number;
        char* string;

        struct {
            struct SObject* objects[MAX_OBJECTS];
            int topObjects;
        };
    };

} Object;


// 3) Init state of virtual mashine (stack and List of all existing objects)
static Object* STACK[MAX_STACK_SIZE];
static int STACK_TOP = 0;
static Object* ROOT = NULL; // <-- List of objects
static int COUNTER = 0;
static int MAX_COUNTER = (1 << 2);


// 4) Base methods of stack (push/pop)
void push(Object* obj) {
    STACK[STACK_TOP++] = obj;
}

Object* pop(void) {
    if (STACK_TOP > 0)
        return STACK[--STACK_TOP];
    else
        return NULL;
}


// 5) methods of strategy GC - reference counting (in particular "Mark And Sweep")
void mark(Object* obj) {
    if (obj->isMarked) return;

    obj->isMarked = true;

    if (obj->type == TWIN)
        for(int i = 0; i < obj->topObjects; i++)
            if (obj->objects[i] != NULL)
                mark(obj->objects[i]);
}

void markAll(void) {
    for (int i = 0; i < STACK_TOP; i++)
        mark(STACK[i]);
}

void sweep(void) {
    Object** tmpObj = &ROOT;

    while (*tmpObj != NULL) {
        if ((*tmpObj)->isMarked == false) {
            Object* unmarked = *tmpObj;

            *tmpObj = unmarked->next;   // <--|
            COUNTER--;                  //    | delete unmarked object from List
            free(unmarked);             // <--|
        }
        else {
            (*tmpObj)->isMarked = false;
            tmpObj = &(*tmpObj)->next;
        }
    }
}


// 6) define Logger and Garbage Collector functions
void Logger(Object* obj, int depthCounter) {
    static Object* loggedObjects[MAX_LIST_SIZE];
    static unsigned long topLoggedObjects = 0;

    if (ALERT_LOGS) {
        if (depthCounter == 0) {
            printf("\tLogger()\n");
            printf("Stack size: [%d]\n", STACK_TOP);
            printf("Existing objects: [%d]\n", COUNTER);
            printf("Data: ");
        }

        switch (obj->type) {
            case NUMBER:
                printf("%lf", obj->number);
                if (depthCounter == 0)
                    printf("\n");
                loggedObjects[topLoggedObjects++] = obj;
                break;
            case STRING:
                printf("\"%s\"", obj->string);
                if (depthCounter == 0)
                    printf("\n");
                loggedObjects[topLoggedObjects++] = obj;
                break;
            case TWIN:
                printf(" {");
                if(obj->topObjects == 0)
                    printf("%s", EMPTY);
                loggedObjects[topLoggedObjects++] = obj;

                for (int i = 0; i < obj->topObjects; i++) {
                    bool isLogged = false;

                    for (int j = 0; j < topLoggedObjects; j++) {
                        if (loggedObjects[j] == obj->objects[i]) {
                            isLogged = true;
                            break;
                        }
                    }

                    if (!isLogged) {
                        Logger(obj->objects[i], depthCounter + 1);
                        if (i < obj->topObjects - 1)
                            printf(", ");
                    }
                    else {
                        printf("cicle_pointer [%p]", obj->objects[i]);
                        if (i < obj->topObjects - 1)
                            printf(", ");
                    }
                }

                printf("}");
                if (depthCounter == 0)
                    printf("\n");
                break;
        }

        if (depthCounter == 0) {
            printf("\tDone!\n");
            topLoggedObjects = 0;
        }
    }
}

void GC(void) {
    int PREV_COUNTER = COUNTER;
    markAll();
    sweep();

    if (ALERT_LOGS) {
        printf("\tGC()\n");
        printf("Collected [%d], Left [%d]\n", PREV_COUNTER - COUNTER, COUNTER);
        printf("\tDone!\n");
    }

    MAX_COUNTER *= 2; // optimisation for performance
}


// 7) methods for creating new objects and owerflow List
Object* newObject(ObjectType type) {
    if (COUNTER >= MAX_COUNTER) GC();

    Object* obj = (Object*)malloc(sizeof(Object));

    obj->isMarked = false;
    obj->type = type;
    if (type == TWIN)
        obj->topObjects = 0;

    obj->next = ROOT; // <--|
    COUNTER++;        //    | add object in List
    ROOT = obj;       // <--|


    return obj;
}

Object* pushNumber(double number) {
    Object* obj = newObject(NUMBER);
    obj->number = number;
    push(obj);
    Logger(obj, 0);
    return obj;
}

Object* pushString(char* string) {
    Object* obj = newObject(STRING);
    obj->string = string;
    push(obj);
    Logger(obj, 0);
    return obj;
}

Object* pushTwin(void) {
    Object* obj = newObject(TWIN);
    push(obj);
    Logger(obj, 0);
    return obj;
}

void pushIntoTwin(Object* objContainer, Object* objItem) {
    if (objContainer->type == TWIN && objContainer->topObjects < MAX_OBJECTS) {
        objContainer->objects[objContainer->topObjects++] = objItem;
        if(ALERT_LOGS)
            Logger(objContainer, 0);
    }
}



int main(void) {
    char str1[32] = "<--Frog And Bug (F&B)-->";
    char str2[32] = "1) Bug *bjjjjjjjj flying...*)))";
    char str3[32] = "2) Frog eated bug's";
    char str4[35] = "3) OMG, What are u doing frog...";



    Object* obj1_twin = pushTwin(); // <-- empty objects
    Object* obj2_twin = pushTwin();

    Object* obj1_str = pushString(str1);
    Object* obj2_str = pushString(str2);

    // <-- trigger GC line (after 4 call push function) ,mast be Collected [0], Left [4] -->

    Object* obj1_nbr = pushNumber((double)42 * 1);
    Object* obj2_nbr = pushNumber((double)42 * 2);


    // below procedures don't increase COUNTER
    pushIntoTwin(obj1_twin, obj1_str);
    pushIntoTwin(obj1_twin, obj2_str);
    pushIntoTwin(obj2_twin, obj1_nbr);
    pushIntoTwin(obj2_twin, obj2_nbr);


    // <-- at this moment COUNTER still equals 4 -->

    pop(); // take {obj1_nbr, obj2_nbr, obj1_str, obj2_str}  from stack
    pop();
    pop();
    pop();


    // <-- create cicle in graph Object's, which incapsulate {obj1_nbr, obj2_nbr, obj1_str, obj2_str} inside yourself -->
    pushIntoTwin(obj2_twin, obj1_twin);
    pushIntoTwin(obj1_twin, obj2_twin);


    // <-- push two time objects in stack, for triggered GC
    pushNumber((double)42 * 1);
    pushNumber((double)42 * 1);


    // <-- trigger GC line (after 4 call push function) ,mast be Collected [0], Left [8] -->
    return 0;
}


