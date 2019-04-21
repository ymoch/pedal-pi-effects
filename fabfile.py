from invoke import task


@task
def install_bcm2835(c):
    """
    See: https://www.electrosmash.com/forum/pedal-pi/202-how-to-start-programming-pedal-pi
    """
    url = 'http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz'
    dir_name = 'bcm2835-1.58'

    c.run(f'rm -rf {dir_name}')

    c.run(f'curl -s "{url}" | tar zxf -')
    with c.cd(dir_name):
        c.run('./configure')
        c.run('make')

    c.sudo(f'bash -c \'cd "{dir_name}" && make install\'')

    c.run(f'rm -rf {dir_name}')


@task
def provision(c):
    install_bcm2835(c)
