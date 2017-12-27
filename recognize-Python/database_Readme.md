主要类（class）database
函数：

1.init初始化：
feature_vector_all储存所有特征向量
category_all储存所有特征向量类别
original_vector_all储存处理前的784维向量
2.get_vec_and_category(filename,feature_vector_all,category_all)
读取txt文件，将文件中的向量，储存在feature_vector_all中，类别储存在category_all中

3.get_(feature_vector,i,j,n)
内置函数，无需用户调用

4.shrink(feature_vector,m=7)
压缩函数，将feature_vector压缩为m*m维向量

5.feature_of_row_col(feature_vector)
内置函数，无需用户调用
6.module(v1)
对v1向量取模

7.dis(v1,v2,model='o')
求v1与v2向量的距离，o为欧拉距离，c为cosine距离

8.find_near_one(v1,list1,model='o')
找到list1集合中与v1向量距离最近的向量

9.load(self,filename,filename_origin,load_type='local',distance_type='c',threshold_value=0.985)
加载函数，最关键的函数。filename为要加载的文件，filename_origin为加载数据的原向量。load_type为local时，加载储存好的处理后的向量。为new时，加载新的训练数据，会对数据进行降维计算，并且进行筛选。threshold_value为距离阈值，在允许范围内，才会加入新的训练数据，否则直接舍弃。

10.load_file
当有多个新的训练数据txt时，可以直接加载文件夹。此函数的参数中填入文件夹名即可。

11.search_feature_vector_for_one_category(self,id_)
子函数，无需用户调用。
11.write_feature_vector_for_one_category(self,id_,save_filename)
将编号为id_的所有原向量，写入save_filename中，检查某一类别的训练数据。
12.write_txt(self,save_filename,save_filename_origin)
将处理后的训练数据与原数据，分别写入save_filename与save_filename_origin文件中。

13.get_index(self,test_list,distance_type='c',if_show_near_num=False)
输出距离test_list向量最近的一个向量的类别序号，if_show_near_num=true时会显示最近向量。

14.delete_one_num(self,num)
删除feature_vector_all中第num个向量，category_all，original_vector_all中会同样删除。
15.clear
内置函数，无需用户调用。