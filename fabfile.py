from invoke import task

@task
def hello(c):
    c.run('echo "Hello, world."')
