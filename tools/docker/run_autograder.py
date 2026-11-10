from docker import from_env
import os
import six
# Run a Docker
client = from_env()
current_path = os.path.abspath('../../')
print(current_path)
container = client.containers.run(
    image='cpp_docker_environment',
    detach=True,
    volumes={
        current_path+'/src': {'bind': '/autograder/submission'},
        current_path+'/autograder': {'bind': '/autograder/source'}
    }
)
# Execute commands
container.exec_run('cp /autograder/source/config.yml /autograder/config.yml')
container.exec_run('./run_autograder')
result = container.exec_run('jq . /autograder/results/results.json')
print(result.output.decode())
# Close and remove container
container.stop()
container.remove()
