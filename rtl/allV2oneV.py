import os

def combine_all_files(folder_path, output_file):
    """
    递归地将指定文件夹及其子文件夹下所有文件的内容合并到一个指定文件中。

    :param folder_path: 要遍历的文件夹路径
    :param output_file: 输出文件的路径
    """
    with open(output_file, 'w') as outfile:
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8', errors='ignore') as infile:
                    outfile.write(infile.read())
                    outfile.write("\n\n")  # 在文件内容之间添加空行

# 使用示例
folder_path = './vsrc/'  # 替换为你的文件夹路径
output_file = 'sources.v'  # 你想要创建的输出文件名
combine_all_files(folder_path, output_file)