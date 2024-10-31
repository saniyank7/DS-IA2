#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 100

typedef struct Component {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    char description[MAX_DESC_LENGTH];
    struct Component* next;
} Component;

Component* createComponent(int id, char* name, float price, int quantity, char* description) {
    Component* newComponent = (Component*)malloc(sizeof(Component));
    newComponent->id = id;
    strcpy(newComponent->name, name);
    newComponent->price = price;
    newComponent->quantity = quantity;
    strcpy(newComponent->description, description);
    newComponent->next = NULL;
    return newComponent;
}

void addComponent(Component** head, int id, char* name, float price, int quantity, char* description) {
    Component* newComponent = createComponent(id, name, price, quantity, description);
    newComponent->next = *head;
    *head = newComponent;
}

void displayComponents(Component* head) {
    if (!head) {
        printf("No components in inventory.\n");
        return;
    }
    printf("\nComponents in Inventory:\n");
    printf("%-5s %-20s %-10s %-10s %s\n", "ID", "Name", "Price", "Quantity", "Description");
    printf("-----------------------------------------------------------\n");
    while (head != NULL) {
        printf("%-5d %-20s $%-9.2f %-10d %s\n", head->id, head->name, head->price, head->quantity, head->description);
        head = head->next;
    }}

typedef struct Stack {
    Component* top;
} Stack;

void initStack(Stack* stack) {
    stack->top = NULL;
}

int isStackEmpty(Stack* stack) {
    return stack->top == NULL;
}

void push(Stack* stack, Component* component) {
    component->next = stack->top;
    stack->top = component;
}

Component* pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        printf("Stack is empty. No component to remove.\n");
        return NULL;
    }
    Component* poppedComponent = stack->top;
    stack->top = stack->top->next;
    return poppedComponent;
}

typedef struct QueueNode {
    Component* component;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

void initQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

void enqueue(Queue* queue, Component* component) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->component = component;
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}

Component* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty. No component to display.\n");
        return NULL;
    }
    QueueNode* temp = queue->front;
    Component* component = temp->component;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return component;
}

void inputComponentDetails(Component** inventory, Stack* stack) {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
    char description[MAX_DESC_LENGTH];

    printf("\n--- Add New Component ---\n");
    printf("Enter component ID: ");
    scanf("%d", &id);

    Component* current = *inventory;
    while (current != NULL) {
        if (current->id == id) {
            printf("Error: Component with ID %d already exists.\n", id);
            return;
        }
        current = current->next;
    }

    printf("Enter component name: ");
    scanf(" %[^\n]", name);
    printf("Enter component price: ");
    scanf("%f", &price);
    printf("Enter component quantity: ");
    scanf("%d", &quantity);
    printf("Enter component description: ");
    scanf(" %[^\n]", description);

    addComponent(inventory, id, name, price, quantity, description);
    push(stack, *inventory);
    printf("Component '%s' added successfully!\n", name);
}

void removeLastComponent(Component** inventory, Stack* stack) {
    Component* removedComponent = pop(stack);
    if (removedComponent) {
        Component** current = inventory;
        while (*current != NULL) {
            if (*current == removedComponent) {
                *current = removedComponent->next;
                printf("Removed Component: ID: %d, Name: %s\n", removedComponent->id, removedComponent->name);
                free(removedComponent);
                return;
            }
            current = &(*current)->next;
        }
        printf("Error: Component not found in inventory!\n");
    }
}

void displayComponentsInQueueOrder(Component* inventory, Queue* queue) {
    for (Component* current = inventory; current != NULL; current = current->next) {
        enqueue(queue, current);
    }

    printf("\n--- Components in Queue ---\n");
    while (queue->front != NULL) {
        Component* component = dequeue(queue);
        if (component) {
            printf("ID: %d, Name: %s, Price: Rs%.2f, Quantity: %d, Description: %s\n",
                   component->id, component->name, component->price, component->quantity, component->description);
        }
    }
}

int main() {
    Component* inventory = NULL;
    Stack stack;
    initStack(&stack);
    Queue queue;
    initQueue(&queue);

    int choice;
    do {
        printf("\n--- Electronics Inventory Management ---\n");
        printf("1. Add Component\n");
        printf("2. Display Components\n");
        printf("3. Remove Last Added Component\n");
        printf("4. Display Components in Order (Queue)\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputComponentDetails(&inventory, &stack);
                break;
            case 2:
                displayComponents(inventory);
                break;
            case 3:
                removeLastComponent(&inventory, &stack);
                break;
            case 4:
                displayComponentsInQueueOrder(inventory, &queue);
                break;
            case 5:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    } while (choice != 5);

    Component* current = inventory;
    while (current != NULL) {
        Component* next = current->next;
        free(current);
        current = next;
    }

    return 0;
}
