## Поиск максимального потока методом предпотоков

### Задание
Найти максимальный поток методом предпотоков. Посчитать число подъемов.

### Алгоритм
В начале сформируем файл ввода “input.txt”. Который в начале можно отредактировать.
* Первая строчка в файле – это количество вершин в графе.
* Вторая исток.
* Третья сток.
* В матрице смежности 0 означает отсутствие ребра {I, j}, а цифра больше 1 – пропускную способность {I, j}.

По желанию выводим граф, используя GraphViz.<br/>

Общая схема алгоритма:
1. На каждом шаге будем рассматривать некоторый предпоток - функцию, которая по свойствам напоминает поток, но не обязательно удовлетворяет закону сохранения потока.
2. На каждом шаге будем пытаться применить какую-либо из двух операций: проталкивание потока или поднятие вершины.
Если на каком-то шаге станет невозможно применить какую-либо из двух операций, то мы нашли требуемый поток.
3. Для каждой вершины определена её высота H, причём H истока = N, H стока = 0, и для любого остаточного ребра (u, v) имеем Hu <= Hv + 1.
4. Для каждой вершины (кроме истока) можно определить её избыток. Вершина с положительным избытком называется переполненной.

*Операция проталкивания (PUSH) выполняется, когда выполнены условия:*
1. u - переполнена
2. ребро u->v принадлежит остаточной сети
3. вершина u выше v, то есть h[u] = h[v] + 1
  
Операция проталкивания заключается, в том что мы находим число вещества для проталкивания (минимальное среди избытка в u и значения остаточной сети для u->v).
Далее добавляем найденное количество вещества в вершину v и отнимает из вершины u. После уменьшаем избыток на u->v и увеличиваем на v->u.<br/>
<br/>

*Операция поднятия (LIFT) выполняется, когда выполняются условия:*
1. u - переполнена
2. нельзя сделать PUSH ни в какую вершину

Операция поднятия, задает новую высоту переполненной вершины, которая равна высоте соседней вершины с минимальной высотой + один.<br/>
<br/>

 В программе есть функция discharge*, которая отвечает за снятие излишка в вершине, в ней выполняются либо функция push, а если невозможно, то функция lift.<br/>
 <br/>
  
1. В главной программе, после графа с пропускной способностью, мы инициализируем транспортную сеть, для которой в функции определяем,
что потока в истоке нет, поток из истока равен пропускной способности, а ребро обратное равно отрицательной пропускной способности.
2. Определяем список потенциальных соседей и заполняем его вершинами, кроме истока и стока.
3. Задаём начальную вершину из этого списка и пока она не окажется в конце списка соседей, мы выполняем несколько действий.
4. Сохраняем высоту вершины перед обслуживанием, обслуживаем вершину (вызываем discharge).
5. После обслуживания, если высота увеличилась, то толкаем вершину в начало списка, переходим к следующей вершине.
6. По завершению цикла, заносим ответ в файл “output.txt”. 

Выводим результат по желанию.


