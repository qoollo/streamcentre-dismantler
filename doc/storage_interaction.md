## Формат запроса с видео фрагментом

POST-запрос, содержащий multipart/form-data

Все поля, кроме самого файла (data) представлены в виде строк:
* stream_id - идентификатор потока
* start_time - начало фрагмента (в виде unix-time)
* stop_time - конец фрагмента 
* container_format - расширение контейнера (mkv, mp4, ...)
* data - фрагмент в бинарном виде

С помощью фласка получить файл можно так: 

```python
file = request.file['data']
# сохранить на диске
file.save(filename)
```

Остальные поля через request.form.get(fieldName), например:
```python
stream_id = int(request.form.get('stream_id'))
```
