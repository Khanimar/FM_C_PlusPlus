function see_result_withmask(filename,N,M)

% Read the original input image
input=imread(filename);
input=double(input);
% Read the result of fast marching
result=zeros(N,M);
fid1=fopen('result.txt','r');

N2=floor(N/2)+1;
M2=floor(M/2)+1;

addition=input(N2,M2);

for i=1:N
    for j=1:M
        result(i,j)=fscanf(fid1,'%f\n',1);
        if result(i,j)~=0
            result(i,j)=result(i,j)+addition;
        end
    end
end

fclose(fid1);

% Read the mask of the result
fid2=fopen('result_mask.txt','r');
mask=zeros(N,M);
% Apply the mask on the input image
for i=1:N
    for j=1:M
        mask(i,j)=fscanf(fid2,'%f\n',1);
        if mask(i,j)==0
           input(i,j)=0;           
        end
    end
end

fclose(fid2);

imtool(result,[]);
output=result;

difference=zeros(N,M);

nomask_count=0;

for i=1:N
    for j=1:M
      if mask(i,j)~=0  
        difference(i,j)=(input(i,j)-output(i,j)).^2; 
        nomask_count=nomask_count+1;
      else
        difference(i,j)=0;  
      end
    end
end

jam=sum(difference(:));
MSE=(1/(nomask_count))*jam;
%MSE=(1/(N*M))*jam;
fprintf('\nMSE=%f\n\n',MSE);

result=mat2gray(result);
imwrite(result,'output.png');

end