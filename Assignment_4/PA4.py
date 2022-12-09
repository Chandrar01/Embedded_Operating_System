# Declare Constants
MAIN_MEMORY_SIZE = 32

# This class holds information about the Page Table Entry
# such as
# 1. process_id: id of process trying to access this page
# 2. page_vpn: virtual page address of the page to be accessed
# 3. page_access: access type viz "w:write" or "r:read"
# 4. page_hit: if page fount in main memory : 1 or page not found: 0
# 5. page_fault: if page not found in the main memory then page fault : 1
#
class PTE:
    def __init__(self, pid, vpn, access):
        self.process_id = pid
        self.page_vpn = vpn
        self.page_access = access
        self.page_hit = 0
        self.page_fault = 0


# This class holds the main page management method
# to add or replace page in the main memory
class PageManagement:
    def __init__(self):
        self.index = None

    def addPageToMainMemory(self, main_memory, pte, visited, hashmap):
        if self.index >= len(main_memory):
            # print("ALERT NO SPACE")
            return

        if (pte.process_id, pte.page_vpn) in visited:
            hashmap[(pte.process_id, pte.page_vpn)][0].page_hit += 1
            return

        main_memory[self.index] = pte
        visited.add((pte.process_id, pte.page_vpn))
        hashmap[(pte.process_id, pte.page_vpn)] = [pte, self.index]
        self.index += 1
        pte.page_fault += 1

    def main(self):
        file = open("sampledata.txt")

        visited = set()
        self.index = 0
        print("process_id \\t page.vpn \\t page.page_hit \\t  page.page_fault")
        hashmap = {}
        main_memory = [0 for _ in range(MAIN_MEMORY_SIZE)]

        for line in file:
            x = line.split("\t")
            pte = PTE(x[0], x[1], x[2])
            self.addPageToMainMemory(main_memory, pte, visited, hashmap)

        for pte in main_memory:
            print(str(pte.page_index) + "\t"
                    + str(pte.page_vpn) + "\t"
                    + str(pte.page_hit) + "\t"
                    + str(pte.page_fault))


pm = PageManagement()
pm.main()
