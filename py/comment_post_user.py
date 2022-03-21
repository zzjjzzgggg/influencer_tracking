data={}
with open('post2.txt','r') as f1:
    for post in f1:
        temp=post.rstrip().split('\t')
        data[temp[0]]=temp[1]
print("post data read")

#a map from userid to username
user={}
with open('user.txt','r') as f:
    for u in f:
        temp=u.rstrip().split('\t')
        user[temp[1]]=temp[2]
print("user data read")

with open('comment_0312.txt','r') as f2, open("result_0312_v3.txt",'w') as fw:
    for comment in f2:
        items = comment.rstrip().split('\t')
        postid, commentuser, mention_name =items[0], items[1], items[2]
        userid = user[mention_name] if mention_name in user else None
        comment_userid = commentuser if postid in data else None
        if userid is not None and comment_userid is not None:
            fw.write("{}\t{}\t{}\t{}\n".format(postid, data[postid], userid, commentuser))
print("ok.")
