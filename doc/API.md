## Начать запись
> /start_rec

#### Параметры
* stream_id - идентификатор потока
* url - url потока

#### Результат:
* 200 - ok

```
{ "result" : "ok" }
```
* 500 - ошибка (поток уже запущен)

```
{ "error_message" : "stream already started" }
```

## Остановить запись
> /stop_rec

#### Параметры
* stream_id - идентификатор потока
#### Результат:
* 200 - ok

```
{ "result" : "ok" }
```
* 404 - ошибка (поток не запущен)

```
{ "error_message" : "stream not started" }
```

## Получить состояние потока
> /get_state

#### Параметры
* stream_id - идентификатор потока

#### Результат:
* 200 - ok

```
{ "is_connected" : <true/false>, "is_running" : <true/false> }
```
* 404 - ошибка (поток не запущен)

```
{ "error_message" : "stream not started" }
```
