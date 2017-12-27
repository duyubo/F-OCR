
# coding: utf-8

# In[3]:


import os
class database:
    def __init__(self):
        self.feature_vector_all=[]
        self.category_all=[]
        self.original_vector_all=[]
    def get_vec_and_category(filename,feature_vector_all,category_all):
        #function for reading txt,load datas
        #feature_vector_all is a list with all feature_vector,category_all is a list
        with open(filename,'r') as f:
            data=f.readlines()
        for line in data:
            line_list=line.split()
            feature_vector_all.append([int(x) for x in line_list[0:-1]])
            category_all.append(int(line_list[-1]))
    def get_(feature_vector,i,j,n):
        #son function for shrink
        #feature_vector is a list of 28*28
        sum_=0
        for col in range(n):
            for row in range(n):
                sum_+=feature_vector[(n*i+col)*28+n*j+row]
        return sum_
    def shrink(feature_vector,m=7):
        #to make the 784 vec to 49 vec
        #feature_vector is a list of 28*28,output is also a list but of 7*7
        output=[]
        for i in range(m):
            for j in range(m):
                output.append(database.get_(feature_vector,i,j,int(pow(len(feature_vector),0.5)/m)))
        return output
    def feature_of_row_col(feature_vector):
        #across and vertical feature
        #feature_vector is a list of 28*28,output is a list with 28+28
        output=[]
        n=int(pow(len(feature_vector),0.5))
        #print('n:',n)
        change_num=0
        for i in range(n):
            for j in range(n-1):
                if feature_vector[n*i+j] != feature_vector[n*i+j+1]:
                    change_num+=1
            output.append(change_num)
            change_num=0
        for j in range(n):
            for i in range(n-1):
                if feature_vector[n*i+j] != feature_vector[n*(i+1)+j]:
                    change_num+=1
            output.append(change_num)
            change_num=0
        return output
    def module(v1):
        #son function for dis
        sum_=0
        for vidu in v1:
            sum_+=pow(vidu,2)
        return pow(sum_,0.5)
    def dis(v1,v2,model='o'):
        #to calculate the distance of two vector
        #v1 v2 are lists
        #model c for cosine,o for oular distance
        if len(v1)!=len(v2):
            print('the lengths of two vec are not equivalent!')
            return -1
        if model=='o':
            sum_=0
            for i in range(len(v1)):
                sum_+=pow(v1[i]-v2[i],2)
            return pow(sum_,0.5)
        else:
            sum_=0
            for i in range(len(v1)):
                sum_+=v1[i]*v2[i] 
            sum_=sum_/(database.module(v1)*database.module(v2))
            return sum_
    def find_near_one(v1,list1,model='o'):
        if model=='o':
            min_dis=database.dis(v1,list1[0],model)
            min_id=0
            for i in range(len(list1)):
                dis_save=database.dis(v1,list1[i],model)
                if dis_save<min_dis:
                    min_id=i
                    min_dis=dis_save
            return min_id
        else:
            max_dis=database.dis(v1,list1[0],model)
            max_id=0
            for i in range(len(list1)):
                dis_save=database.dis(v1,list1[i],model)
                if dis_save>max_dis:
                    max_id=i
                    max_dis=dis_save
            return max_id
    def handle_vector(feature_vector,m_for_shrink=7):
        output=[]
        output.extend(database.shrink(feature_vector,m_for_shrink))
        output.extend(database.feature_of_row_col(feature_vector))
        return output
    def load_file(self,path,load_type='new',distance_type='c',threshold_value=0.985):
        for root , dirs, files in os.walk(path):
            #print('root:',root)
            #print('files:',files)
            #print('dirs:',files)
            for name in files:
                file_name=root+'/'+name
                try:
                    self.load(file_name,file_name,load_type,distance_type,threshold_value)
                except:
                    print('cann\'t open:',name)
        print('finished!')
    def load(self,filename,filename_origin,load_type='local',distance_type='c',threshold_value=0.985):
        #local means the data saved before,new means new datas
        #model c for cosine,o for oular distance
        if distance_type!='c' and distance_type!='o':
            print('wrong distance_type!')
            return
        else:
            pass
        if load_type=='local':
            database.get_vec_and_category(filename,self.feature_vector_all,self.category_all)
            c=[]
            database.get_vec_and_category(filename_origin,self.original_vector_all,c)
            c=[]
            if len(self.feature_vector_all) != len(self.original_vector_all):
                print('worng:the length of two documents isn\t equivalent')
                self.feature_vector_all=[]
                self.original_vector_all=[]
                self.category_all=[]
            else:
                print('load success!')
                print('the length of training data:',len(self.feature_vector_all))
        elif load_type == 'new':
            #self.original_vector_all=[]
            #self.original_category=[]
            print('start load:',filename)
            vector_784_list=[]
            category_new_list=[]
            database.get_vec_and_category(filename,vector_784_list,category_new_list)
            n_new_data=len(vector_784_list)
            m_count=0
            if n_new_data==0:
                print('no data to load!')
                return
            #self.original_vector_all.extend(vector_784_list)
            if len(self.feature_vector_all)==0:
                self.feature_vector_all.append(database.handle_vector(vector_784_list[0]))
                self.original_vector_all.append(vector_784_list[0])
                self.category_all.append(category_new_list[0])
                m_count+=1
            for index,vector_784 in enumerate(vector_784_list):
                after_handle_vector=database.handle_vector(vector_784)
                most_near_id=database.find_near_one(after_handle_vector,self.feature_vector_all,distance_type)
                dis_save=database.dis(after_handle_vector,self.feature_vector_all[most_near_id],distance_type)
                if distance_type=='c': 
                    if dis_save < threshold_value:
                        m_count+=1
                        self.feature_vector_all.append(after_handle_vector)
                        self.original_vector_all.append(vector_784_list[index])
                        self.category_all.append(category_new_list[index])
                else:
                    if dis_save > threshold_value:
                        m_count+=1
                        self.feature_vector_all.append(after_handle_vector)
                        self.category_all.append(category_new_list[index])
            print('load new data length:',n_new_data)
            print('valid new data length:',m_count) 
            vector_784_list=[]
            category_new_list=[]
        else:
            print('wrong load type!')
            return
    def search_feature_vector_for_one_category(self,id_):
        output_id=[]
        for num in range(len(self.category_all)):
            if self.category_all[num] == id_:
                output_id.append(num)
        return output_id
    def write_feature_vector_for_one_category(self,id_,save_filename):
        self.clear()
        output=self.search_feature_vector_for_one_category(id_)
        
        if len(output) == 0:
            print('no data')
            return
        
        f=open(save_filename,'w')
        for num in output:
            for v in self.original_vector_all[num]:
                f.write(str(v)+' ')
            f.write(str(num))
            f.write('\n')
        f.close()
        print('finished write!')
    def write_txt(self,save_filename,save_filename_origin):
        self.clear()
        if len(self.feature_vector_all)!=len(self.category_all):
            print('the lengths of feature_vector_all and category_all are not equivalent!')
            return
        f=open(save_filename,'w')
        for i in range(len(self.feature_vector_all)):
            for w in self.feature_vector_all[i]:
                f.write(str(w)+' ')
            f.write(str(self.category_all[i])+'\r\n')
        f.close()
        f=open(save_filename_origin,'w')
        for i in range(len(self.original_vector_all)):
            for w in self.original_vector_all[i]:
                f.write(str(w)+' ')
            f.write(str(self.category_all[i])+'\r\n')
        f.close()
        print('finished write,write data number:',len(self.feature_vector_all))
    def get_index(self,test_list,distance_type='c',if_show_near_num=False):
        test=database.handle_vector(test_list)
        most_near_id=database.find_near_one(test,self.feature_vector_all,distance_type)
        if if_show_near_num:
            print(most_near_id)
        return self.category_all[most_near_id]
    def delete_one_num(self,num):
        self.feature_vector_all[num]=[]
        self.category_all[num]=-1
        self.original_vector_all[num]=[]
    def delete(self,num):
        if type(num) == list:
            for n in num:
                self.delete_one_num(n)
        else:
            self.delete_one_num(num)
        print('finished')
    def clear(self):
        while [] in self.feature_vector_all:
            self.feature_vector_all.remove([])
        while [] in self.original_vector_all:
            self.original_vector_all.remove([])
        while -1 in self.category_all:
            self.category_all.remove(-1)


# In[32]:


#load new
rec=database()
rec.load('text/1.txt','xx',load_type='new',distance_type='c',threshold_value=0.985)


# In[71]:


#save
rec.write_txt('td/fd.txt','td/o.txt')


# In[16]:


#load local
rec=database()
rec.load('td/fd.txt','td/o.txt')
#rec.load('text/save8.txt',load_type='new',distance_type='c',threshold_value=0.985)


# In[73]:


#extend
rec.load('text/2.txt','xx',load_type='new',distance_type='c',threshold_value=0.985)


# In[77]:


#save after extend
rec.write_txt('td/fd.txt','td/o.txt')


# In[10]:


#check one category
rec.write_feature_vector_for_one_category(126,'111.txt')
rec.write_feature_vector_for_one_category(127,'222.txt')


# In[76]:


#remove one vec
rec.delete(1)


# In[4]:


#start train
rec=database()
#rec.load('td/fd.txt','td/o.txt')


# In[9]:


#save
rec.write_txt('td/fd.txt','td/o.txt')


# In[8]:


#load new file
path='sample/unused'
#path2='sample/unused/latex'
rec.load_file(path)


# In[6]:


rec.load_file(path1)
