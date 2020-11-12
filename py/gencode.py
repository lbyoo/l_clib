import time

module = "set"
prefix = "l_"

h_file = "{}{}.h".format(prefix, module)
c_file = "{}{}.c".format(prefix, module)
comm_inc = ["l_mem.h", "l_lock.h", "l_log.h", "l_mem.h", "l_type.h"]
comm_head = """ \
/*****************************************************************
 * file: {}
 * author: libaoyu
 * date: {}
 * desc: 
 ******************************************************************/
"""
comm_inc.append(h_file)
with open("src/inc/%s" % h_file, "w") as f:
    f.write("#ifndef _{}_\n".format(h_file.upper().replace(".","_")))
    f.write("#define _{}_\n\n".format(h_file.upper().replace(".","_")))

    f.write("#endif\n")

with open("src/%s" % c_file, "w") as f:
    f.write(comm_head.format(c_file, time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())))
    for inc in comm_inc:
        f.write("#include \"%s\"\n" % inc)