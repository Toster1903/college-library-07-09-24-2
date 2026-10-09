from pathlib import Path
import subprocess, tempfile, unittest
APP = Path(__file__).resolve().parents[1] / 'library'
class LibraryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(); self.directory = Path(self.temp.name)
    def tearDown(self): self.temp.cleanup()
    def run_app(self, text, code=0):
        result = subprocess.run([str(APP)], input=text, text=True, capture_output=True, cwd=self.directory)
        self.assertEqual(result.returncode, code, result.stderr)
        return result.stdout + result.stderr
    def test_full_cycle(self):
        out = self.run_app('1\n1\nУчебник\n2024\nАвтор\n123\n2\n2\nНаука\n2025\n9\n10\n3\n10\nИван Иванов\n4\n1\n5\n10\n1\n5\n10\n1\n8\n7\n9\n0\n')
        for text in ['Книга добавлена','Журнал добавлен','Читатель зарегистрирован','Учебник','Издание выдано','Издание уже выдано','Выдача #1','Данные сохранены']: self.assertIn(text,out)
        self.assertEqual((self.directory/'loans.txt').read_text(), '1;10;1;1\n')
        out = self.run_app('8\n6\n1\n6\n1\n7\n8\n9\n0\n')
        for text in ['Выдача #1','Издание возвращено','Издание уже возвращено','Нет активных выдач','[доступно]']: self.assertIn(text,out)
        self.assertEqual((self.directory/'loans.txt').read_text(),'1;10;1;0\n')
        self.assertIn('Нет активных выдач', self.run_app('8\n0\n'))
    def test_input_and_duplicates(self):
        out = self.run_app('10\nabc\n1\n0\n1\n\n ; \nКнига\n1499\n2101\n1500\nАвтор\nISBN\n1\n1\n2\n1\n3\n9\nИмя\n3\n9\n5\n99\n5\n9\n99\n6\n99\n9\n0\n')
        for text in ['Неизвестная команда','Введите целое число','Строка должна быть непустой','ID уже существует','Билет уже существует','Читатель не найден','Издание не найдено','Выдача не найдена']: self.assertIn(text,out)
    def test_month_boundaries(self):
        out = self.run_app('2\n1\nЖурнал\n2100\n1\n0\n13\n12\n9\n0\n')
        self.assertIn('от 1 до 12',out)
        self.assertEqual((self.directory/'items.txt').read_text(),'Magazine;1;Журнал;2100;1;1;12\n')
    def test_corrupt_data(self):
        for content in ['Book;1;Книга;1499;1;Автор;ISBN\n','Book;1;Книга;2000;0;Автор;ISBN\n','Magazine;1;Журнал;2000;1;1;13\n','bad\n']:
            (self.directory/'items.txt').write_text(content)
            self.assertIn('Ошибка загрузки',self.run_app('',1))
            self.assertEqual((self.directory/'items.txt').read_text(),content)
    def test_input_module(self):
        # Значения из УП5: 0, 1, 9, 10 и пустая строка проверяются через реальный ввод меню.
        out=self.run_app('0\n'); self.assertIn('Выход',out)
        out=self.run_app('\n9\n10\n0\n'); self.assertIn('Введите целое число',out); self.assertIn('Неизвестная команда',out)
if __name__ == '__main__': unittest.main(verbosity=2)
