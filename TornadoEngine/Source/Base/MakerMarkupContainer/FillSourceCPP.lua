function WriteLineCPP()
  fileCPP:write("//---------------------------------------------------------\n");
end;
---------------------------------------------------------------------
function WriteFormula(node)
  -- формула
  if #node==0 then
    fileCPP:write(" C ");
  else
    fileCPP:write(" Arr( ");
  end;
  
  for i = 1, #node do
    local value = node[i];
    if type(value)=="table" then
      WriteFormula(value);
    end;
  end;

  if #node>0 then 
    fileCPP:write(" ) ");
  end;
end;
---------------------------------------------------------------------
function WriteGlobalStr(node)
  local nameFunc = node["nameFunc"];
  local name     = node["name"];
  if nameFunc~=nil then
    fileCPP:write("  const char* g_p"..nameFunc.." = \""..name.."\";\n");
  end;
    
  for i = 1, #node do
    local value = node[i];
    if type(value)=="table"then
      WriteGlobalStr(value);
    end;
  end;
end;
---------------------------------------------------------------------
function WritePushDesc(node, num_tab)
  
  local str_tab_pre = "";
  local str_tab     = "  ";
  for i_tab = 1, num_tab do
    str_tab     = str_tab.."  ";
    str_tab_pre = str_tab_pre.."  ";
  end;
  
  local nameFunc   = node["nameFunc"];
  local type_node  = node["type"];
  local type_const = node["type_const"];
  if nameFunc~=nil and type_node~=nil then
    fileCPP:write(str_tab);
    fileCPP:write("IMarkUpContainer::TDesc* p"..nameFunc.." = new IMarkUpContainer::TDesc;\n");
    fileCPP:write(str_tab);
    fileCPP:write("p"..nameFunc.."->name = g_p"..nameFunc..";\n");
    fileCPP:write(str_tab);
    fileCPP:write("p"..nameFunc.."->typeConst = \""..type_const.."\";\n");
    fileCPP:write(str_tab);
    fileCPP:write("p"..nameFunc.."->size = sizeof("..type_node..");\n");
    for i = 1, #node do
      local value = node[i];
      if type(value)=="table" then
        WritePushDesc(value, num_tab + 1);
      end;     
    end;
    local parent = node["parent"];
    if parent==nil then
      fileCPP:write(str_tab_pre);
      fileCPP:write("mRootDesc->list.push_back(p"..nameFunc..");\n");
      fileCPP:write("\n");
    else
      local nameFunc_parent = parent["nameFunc"];
      fileCPP:write(str_tab_pre);
      fileCPP:write("p"..nameFunc_parent.."->list.push_back(p"..nameFunc..");\n");
    end;
  end;
end;
---------------------------------------------------------------------
function WriteBody_GET_N(name, type_param, arr_name, i)
  local tabs = "";
  for i_tab = 1,i do
    tabs = "  "..tabs;
  end;
  
  local name_index = arr_name[#arr_name - i + 1];
  if name_index~=nil then
    fileCPP:write(tabs);
    fileCPP:write("if(mMarkUp->Enter(g_p"..name_index..", index"..name_index.." ))\n");
    
    fileCPP:write(tabs);
    fileCPP:write("{\n");
    
    WriteBody_GET_N(name, type_param, arr_name, i + 1);
    
    fileCPP:write(tabs);
    fileCPP:write("}\n");
    
  else
    fileCPP:write(tabs);
    fileCPP:write(type_param.."* p = ("..type_param.."*)mMarkUp->GetPtrConst(g_p"..name..");\n");
    
    -- проверка результата
    fileCPP:write(tabs)fileCPP:write("if(p==NULL)\n");
    fileCPP:write(tabs)fileCPP:write("{\n");
    fileCPP:write(tabs)fileCPP:write("  BL_FIX_BUG();\n");
    fileCPP:write(tabs)fileCPP:write("  return "..type_param.."();\n");
    fileCPP:write(tabs)fileCPP:write("}\n");
    
    fileCPP:write(tabs);
    fileCPP:write(type_param.." res = *p;\n");
    
    fileCPP:write(tabs);
    fileCPP:write("return res;\n");
  end;
end;
---------------------------------------------------------------------
function WriteBody_GET(name, type_param, arr_name)
  fileCPP:write("{\n");
  fileCPP:write("  mMarkUp->EnterRoot();\n");
  
  WriteBody_GET_N(name, type_param, arr_name, 1);
  
  if #arr_name>0 then
    fileCPP:write("  return "..type_param.."();\n");
  end
  fileCPP:write("}\n");
  WriteLineCPP();
end
---------------------------------------------------------------------
function WriteBody_GET_COUNT_N(name, type_param, arr_name, i)
  local tabs = "";
  for i_tab = 1,i do
    tabs = "  "..tabs;
  end;
  
  local name_index = arr_name[#arr_name - i + 1];
  if name_index~=nil then
    fileCPP:write(tabs);
    fileCPP:write("if(mMarkUp->Enter(g_p"..name_index..", index"..name_index.." ))\n");
    
    fileCPP:write(tabs);
    fileCPP:write("{\n");
    
    WriteBody_GET_COUNT_N(name, type_param, arr_name, i + 1);
    
    fileCPP:write(tabs);
    fileCPP:write("}\n");
    
  else
    fileCPP:write(tabs);
    fileCPP:write("int cnt = mMarkUp->GetCount(g_p"..name..");\n");
   
    fileCPP:write(tabs);
    fileCPP:write("return cnt;\n");
  end;
end;
---------------------------------------------------------------------
function WriteBody_GET_COUNT(name, type_param, arr_name)
  fileCPP:write("{\n");
  fileCPP:write("  mMarkUp->EnterRoot();\n");
  
  WriteBody_GET_COUNT_N(name, type_param, arr_name, 1);
  
  if #arr_name>0 then
    fileCPP:write("  return 0;\n");
  end;
  
  fileCPP:write("}\n");
  WriteLineCPP();
end
---------------------------------------------------------------------
function WriteBody_SET_N(name, type_param, arr_name, i)
  local tabs = "";
  for i_tab = 1,i do
    tabs = "  "..tabs;
  end;
  
  local name_index = arr_name[#arr_name - i + 1];
  if name_index~=nil then
    fileCPP:write(tabs);
    fileCPP:write("if(mMarkUp->Enter(g_p"..name_index..", index"..name_index.." ))\n");
    
    fileCPP:write(tabs);
    fileCPP:write("{\n");
    
    WriteBody_SET_N(name, type_param, arr_name, i + 1);
    
    fileCPP:write(tabs);
    fileCPP:write("}\n");
    
  else
    fileCPP:write(tabs);
    fileCPP:write(type_param.."* p = ("..type_param.."*)mMarkUp->GetPtrConst(g_p"..name..");\n");
    
    -- проверка результата
    fileCPP:write(tabs) fileCPP:write("if(p==NULL)\n");
    fileCPP:write(tabs) fileCPP:write("{\n");
    fileCPP:write(tabs) fileCPP:write("  BL_FIX_BUG();\n");
    fileCPP:write(tabs) fileCPP:write("  return;\n");
    fileCPP:write(tabs) fileCPP:write("}\n");
    
    fileCPP:write(tabs) fileCPP:write("*p = v;\n");
  end
end
---------------------------------------------------------------------
function WriteBody_SET(name, type_param, arr_name)
  fileCPP:write("{\n");
  fileCPP:write("  mMarkUp->EnterRoot();\n");
  
  WriteBody_SET_N(name, type_param, arr_name, 1);
  
  fileCPP:write("}\n");
  WriteLineCPP();
end;
---------------------------------------------------------------------
function WriteBody_SET_COUNT_N(name, type_param, arr_name, i)
  local tabs = "";
  for i_tab = 1,i do
    tabs = "  "..tabs;
  end;
  
  local name_index = arr_name[#arr_name - i + 1];
  if name_index~=nil then
    fileCPP:write(tabs);
    fileCPP:write("if(mMarkUp->Enter(g_p"..name_index..", index"..name_index.." ))\n");
    
    fileCPP:write(tabs);
    fileCPP:write("{\n");
    
    WriteBody_SET_COUNT_N(name, type_param, arr_name, i + 1);
    
    fileCPP:write(tabs);
    fileCPP:write("}\n");
  else
    fileCPP:write(tabs);
    fileCPP:write("mMarkUp->SetCount(g_p"..name..", v);\n");
  end
end
---------------------------------------------------------------------
function WriteBody_SET_COUNT(name, type_param, arr_name)
  fileCPP:write("{\n");
  fileCPP:write("  mMarkUp->EnterRoot();\n");
  
  WriteBody_SET_COUNT_N(name, type_param, arr_name, 1);
    
  fileCPP:write("}\n");
  WriteLineCPP();
end
---------------------------------------------------------------------
function WriteSetDefValue()
  for i = 1, #arr_stack_desc do
    local value = arr_stack_desc[i];
    if value~=nil then
      local nameFunc = value["nameFunc"];
      local def_value = value["def_value"];
      if def_value~=nil then
        fileCPP:write("  Set"..nameFunc.."("..def_value..");\n");
      end
    end;
  end;
end;
---------------------------------------------------------------------
function WriteMethodBodyByNode(node)
  size_stack_methods = size_stack_methods + 1;
  local arr_name = {};
  -- формирование методов
  local nameFunc_node = node["nameFunc"];
  local type_node = node["type"];
  local cnt_child = #node;
  ---------------------------------
  local str_index_param = "";
  local parent = node["parent"];
  for i=1,size_stack_methods do
    if parent==nil then
      break;
    end;
    local nameFunc_parent = parent["nameFunc"];
    if #str_index_param==0 then
      str_index_param = "int index"..nameFunc_parent;
    else
      str_index_param = "int index"..nameFunc_parent..", "..str_index_param;
    end
    parent = parent["parent"];
    
    arr_name[i] = nameFunc_parent;
  end
  ---------------------------------
  if cnt_child==0 then -- Const
    fileCPP:write(type_node.." T"..name_packet.."::Get"..nameFunc_node.."("..str_index_param..")\n");
    WriteBody_GET(nameFunc_node, type_node, arr_name); -- тело метода
    if #str_index_param~=0 then
      str_index_param = str_index_param..", ";
    end;
    fileCPP:write("void T"..name_packet.."::Set"..nameFunc_node.."("..str_index_param..type_node.." v)\n");
    WriteBody_SET(nameFunc_node, type_node, arr_name); -- тело метода
  else -- Arr
    fileCPP:write("int T"..name_packet.."::GetCount"..nameFunc_node.."("..str_index_param..")\n");
    WriteBody_GET_COUNT(nameFunc_node, type_node, arr_name); -- тело метода
    if #str_index_param~=0 then
      str_index_param = str_index_param..", ";
    end;
    fileCPP:write("void T"..name_packet.."::SetCount"..nameFunc_node.."("..str_index_param.."int v)\n");
    WriteBody_SET_COUNT(nameFunc_node, type_node, arr_name); -- тело метода
  end;
  ---------------------------------
  for i = 1, #node do
    local value = node[i];
    if type(value)=="table" then
      WriteMethodBodyByNode(value);
    end;
  end;
  size_stack_methods = size_stack_methods - 1;
end;
---------------------------------------------------------------------
function FillSourceCPP()
  fileCPP:write("#include \""..name_packet..".h\"\n");
  
  for index_add_include, str_include in pairs(add_include_in_cpp) do
    fileCPP:write("#include \""..str_include.."\"\n");
  end;
  
  fileCPP:write("\n");
  ------------------------------------------
  -- типа: формула пакета Root(C, Arr(C), C)
  fileCPP:write("// формула пакета Root(");
  for i = 1, #arr_stack_desc do
    local node = arr_stack_desc[i];
    if type(node)=="table" then
      WriteFormula(node);
    end;
  end;
  fileCPP:write(")\n");
  fileCPP:write("\n");
  ------------------------------------------  
  -- namespace пакета
  fileCPP:write("namespace ns"..name_packet.."\n");
  fileCPP:write("{\n");
  for i = 1, #arr_stack_desc do
    local node = arr_stack_desc[i];
    if type(node)=="table" then
      WriteGlobalStr(node);
    end;
  end;
  fileCPP:write("}\n");
  ------------------------------------------
  fileCPP:write("\n");
  fileCPP:write("using namespace std;\n");
  fileCPP:write("using namespace ns"..name_packet..";\n");
  fileCPP:write("\n");
  fileCPP:write("T"..name_packet.."::T"..name_packet.."()\n");
  fileCPP:write("{\n");
  -- вставка иерархии в MarkUp
  -----------------------------
  for i = 1, #arr_stack_desc do
    local node = arr_stack_desc[i];
    if type(node)=="table" then
      WritePushDesc(node, 1);
    end;
  end;
  fileCPP:write("  mMarkUp->SetMarkUp(mRootDesc);\n");
  fileCPP:write("\n");  
  fileCPP:write("  SetDefValue();\n");
  -----------------------------
  fileCPP:write("}\n");
  WriteLineCPP();
  fileCPP:write("T"..name_packet.."::~T"..name_packet.."()\n");
  fileCPP:write("{\n");
  fileCPP:write("\n");
  fileCPP:write("}\n");
  WriteLineCPP();
  fileCPP:write("void T"..name_packet.."::SetDefValue()\n");
  fileCPP:write("{\n");
  WriteSetDefValue();
  fileCPP:write("}\n");
  WriteLineCPP();
  fileCPP:write("int  T"..name_packet.."::GetID()\n");
  fileCPP:write("{\n");
  fileCPP:write("  return "..id_packet..";\n");
  fileCPP:write("}\n");
  WriteLineCPP();  
  fileCPP:write("std::string T"..name_packet.."::GetName()\n");
  fileCPP:write("{\n");
  fileCPP:write("  return \""..full_name_packet.."\";\n");
  fileCPP:write("}\n");
  WriteLineCPP();  
  -- перебор методов
  for i = 1, #arr_stack_desc do
    local node = arr_stack_desc[i];
    if type(node)=="table" then
      WriteMethodBodyByNode(node);
    end;
  end;
  
  fileCPP:write("\n");
  fileCPP:flush();
  fileCPP:close();
end;
---------------------------------------------------------------------
