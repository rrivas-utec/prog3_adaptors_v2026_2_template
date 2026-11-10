from docker import from_env, errors
import six


def close_container(container_name):
    client = from_env()
    try:
        container = client.containers.get(container_name)
        container.stop()
        container.remove()
        print(f'Container: [{container_name}] has been stopped and removed...')
    except errors.NotFound:
        print(f'Container: [{container_name}] has not been found...')


if __name__ == '__main__':
    cnt_name = 'cpp_docker_environment'
    close_container(cnt_name)
