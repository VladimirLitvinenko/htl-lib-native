#ifndef ERROR_LITERAL_H
#define ERROR_LITERAL_H

#define ST_ASSERT_ABSTRACT_METHOD      "Необходимо перегрузить реализацию метода."
#define ST_ASSERT_BASE_CLASS_VALIDATOR "Класс параметра не унаследован от класса,требуемого условием метода."
#define ST_ASSERT_TEMPLATE_VALIDATOR   "Не выполнено правило проверки типа шаблона класса."
#define ST_ASSERT_EQUALS_TYPES         "Типы класса идентичен типу параметра аналогичного метода."
#define ST_ASSERT_ONLY_CORE_APP        "Тип класса сервиса не должен содержать графический интерфейс."
#define MSG_ERR_TEMPLATE_TYPE          "Тип шаблона ограничен условием объявления класса."
#define MSG_ERR_CONSTRUCT_TYPE         "Тип должен иметь конструктор инициализации."
#define MSG_ERR_COPY_ASSIGGNABLE_TYPE  "Тип должен иметь конструктор присваивания."
#define MSG_ERR_ENUM_TYPE              "Тип должен быть перечисляемым (enum)."
#define MSG_ERR_DB_TABLE               "Несоответствие количества таблиц в базе данных."

#endif // ERROR_LITERAL_H
