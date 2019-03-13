%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
% Copyright (c) 2018, Maryam Khanian, Ali Sharifi Boroujerdi
% All rights reserved.

% Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

% 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

% 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
