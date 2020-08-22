Тестовое задание:

```
1. Simple API
Реализовать простейший REST API клиент делающий 1 запрос, аналогичный такому:

curl -X POST http://localhost:8000/api/json/v2 -L -d '{"id":0, "jsonrpc":"2.0","method":"systemInfo","params":[]}'

Предусмотреть интерфейс, через который можно передать другой метод, не systemInfo, 
т.е. Вызов должен быть оформлен отдельной функцией.

Для решения используем С99. не используем запуск курла из-под решения, но можно использовать его библиотеки.
```

## Зависимости

* libcurl
* libjson-c

## Сборка

```bash
$ cd rest_cl
$ mkdir build
$ cd build/
$ cmake ..
$ make
```

## Запуск

Для начала запускаем тестовый сервер

```bash
$ python3 rest_sv/Server.py
```

Клиент может быть запущен со следующими опциями.

```bash
$ ./Client -h
Usage:
./Client
	 -h  Print usage
	 -v  Verbose
	 -u [url]	Default URL: http://localhost:8000/api/json/v2 
	 -m [method name]
Possible methods:
* systemInfo
* hardwareInfo
```

В базовом случае:

```bash
$ ./Client -m systemInfo 
Ok!
```

Один запуск инициирует передачу только одного (последнего) метода.

## Добавление дополнительных методов

Для добавления дополнительного метода необходимо в файле ```methods.c``` добавить реализацию метода.

```c
void someMethod(char* result)
{
    // Реализация
}
```

И включить его в список ```methods```.

```c
static method_t methods[] =
{
    { "systemInfo", systemInfo },
    { "hardwareInfo", hardwareInfo },
    { "someMethod", someMethod }
    /* other methods */
};
```

