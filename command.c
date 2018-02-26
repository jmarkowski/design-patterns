#include <stdio.h>
#include <stdlib.h>

/**
 * Intent
 * - Encapsulate a request as an object, thereby letting you parameterize
 *   clients with different requests, queue or log requests, and support
 *   undoable operations.
 */

/**
 * Use the Command pattern when
 * - You want to parameterize objects by an action to perform. You can express
 *   such parameterization in a procedural language with a "callback" function,
 *   that is a function that's registered somewhere to be called at a later
 *   point. Commands are an object-oriented replacement for callbacks.
 * - You want to specify, queue, and execute requests at different times. A
 *   Command ojbject can have a lifetime independent of the original request.
 * - You want to support undo. The Command's Execute operation can store state
 *   for reversing its effects in the command itself. The Command interface must
 *   have an added Unexecute operation that reverses the effects of a previous
 *   call to Execute. Executed commands are stored in a history list.
 *   Unlimited-level undo and redo is achieved by traversing this list backwards
 *   and forwards calling Unexecute and Execute, respectively.
 * - You want to support logging changes so that they can be reapplied in case
 *   of a system crash. By augmenting the Command interface with load and store
 *   operations, you can keep a persistent log of changes. Recovering from a
 *   crash involves reloading logged commands from disk and reexecuting them
 *   with the Execute operation.
 * - You want to structure a system around high-level operations built on
 *   primitives operations. Such a structure is common in information systems
 *   that support transactions. A transaction encapsulates a set of changes to
 *   data. The Command pattern offers a way to model transactions. Commands have
 *   a commoin interface, letting you invoke all transactions the same way. The
 *   pattern also makes it easy to extend the system with new transactions.
 */
/* i.e. The object that will execute a command's action */
typedef struct Receiver_s {
    void (*action)(void);
} Receiver_t;

void receiverAction(void)
{
    printf("Receiver action performed!\n");
}

Receiver_t * newReceiver(void)
{
    Receiver_t *receiver = (Receiver_t *) malloc(sizeof(Receiver_t));

    receiver->action = receiverAction;

    return receiver;
}

typedef struct Command_s {
    Receiver_t *receiver;
    void (*execute)(struct Command_s *command);
} Command_t;

void commandExecute(Command_t *command)
{
    printf("The command is executing, calling the receiver...\n");
    command->receiver->action();
}

Command_t * newCommand(Receiver_t *receiver)
{
    Command_t *command = (Command_t *) malloc(sizeof(command));

    command->receiver = receiver;
    command->execute = commandExecute;

    return command;
}

/* i.e. The object that will handle a request */
typedef struct Invoker_s {
    Command_t *requestHandler;
    void (*storeCommand)(struct Invoker_s *invoker, Command_t *concreteCommand);
    void (*doCommand)(struct Invoker_s *invoker);
} Invoker_t;

void invokerSetCommand(Invoker_t *invoker, Command_t *concreteCommand)
{
    invoker->requestHandler = concreteCommand;
}

void invokerHandleRequest(Invoker_t *invoker)
{
    printf("The invoker has received a request to handle a command...\n");
    invoker->requestHandler->execute(invoker->requestHandler);
}

Invoker_t * newInvoker(void)
{
    Invoker_t *invoker = (Invoker_t *) malloc(sizeof(Invoker_t));

    invoker->storeCommand = invokerSetCommand;
    invoker->doCommand = invokerHandleRequest;

    return invoker;
}

int main(void)
{
    Receiver_t * receiver = newReceiver();
    Command_t * concreteCommand = newCommand(receiver);
    Invoker_t * invoker = newInvoker();

    invoker->storeCommand(invoker, concreteCommand);

    invoker->doCommand(invoker);

    return 0;
}
