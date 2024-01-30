from multiprocessing import Process, freeze_support
import subprocess

def run_script(script):
    subprocess.run(['python', script])

if __name__ == '__main__':
    freeze_support()  

    process1 = Process(target=run_script, args=('CloudRetriever2.py',))
    process2 = Process(target=run_script, args=('app.py',))

    process1.start()
    process2.start()

    process1.join()
    process2.join()
