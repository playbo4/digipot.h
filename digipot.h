#pragma once

class DigiPot {
  public:
    DigiPot(uint8_t cs, uint8_t ud, uint8_t inc) {
#if defined (__AVR__)
      _inc_ptr = portOutputRegister(digitalPinToPort(inc));		// Получаем указатель на порт 
      _inc_msk = digitalPinToBitMask(inc);						// Получаем битовую маску
      _pin_cs = cs;												// Получаем остальные пины
      _pin_ud = ud;

      pinMode(cs, OUTPUT);		// Все пины на выход
      pinMode(ud, OUTPUT);
      pinMode(inc, OUTPUT);
#else
      _pin_cs = cs;
      _pin_ud = ud;
      _pin_inc = inc;

      pinMode(cs, OUTPUT);
      pinMode(ud, OUTPUT);
      pinMode(inc, OUTPUT);
#endif
	 
      digitalWrite(cs, HIGH);	 // Эти два	сразу в положение HIGH
      digitalWrite(inc, HIGH);
    }

    void write(int8_t value) {				// Функция для прямой записи сопротивления в шагах
      increment(value - _digipot_value);	// Сдвинуть на разницу между текущим и требуемым значением
      _digipot_value = value;				// Обновить текущее значение
    }

    void increment(int8_t inc) {			// Функция для сдвига положеняи потенциометра
      bool direction;						// Флаг для установки направления
      if (inc < 0) {						// Если сдвиг отрицательный
        direction = false;					// Флаг - false
        inc *= -1;							// Убираем отриц. знак	
      } else {								// Иначе
        direction = true;					// Флаг false
      }
      digitalWrite(_pin_cs, LOW);			// Начинаем передачу
      digitalWrite(_pin_ud, direction);	    // Устанавливаем направление
      for (uint8_t i = 0; i < inc * 2; i++) {	// Цикл отправки
#if defined (__AVR__)						// Если на указателе
        *_inc_ptr ^= _inc_msk;				// Инвертируем пин
        asm volatile("NOP");				// Небольшая задержка
        asm volatile("NOP");
#else										// Универсальный вариант
        digitalWrite(_pin_inc, !digitalRead(_pin_inc)); // Инверсия пина (можно через флаг - быстрее)
#endif
      }
      digitalWrite(_pin_cs, HIGH);			// Окончание передачи
    }

    void reset(void) {						// Функция для скручивания в мин. положение
      increment(-100);
    }

  private:
#if defined (__AVR__)
    uint8_t volatile *_inc_ptr = 0;			// Указатель
    uint8_t volatile _inc_msk = 0;			// Маска
    uint8_t _pin_ud = 0;					// Прочие пины
    uint8_t _pin_cs = 0;
#else
    uint8_t _pin_cs = 0;
    uint8_t _pin_ud = 0;
    uint8_t _pin_inc = 0;
#endif
    uint8_t _digipot_value = 0;
};